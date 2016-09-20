
Coordinate Systems and Transformation Flow
..........................................

There are several coordinate systems in common use in astronomy.  When an
astronomer specifies a target coordinate, that information often needs to be
transformed into another system before it can be used to command the telescope
to a [Az, El] location and for telescope tracking.  The general implementation
strategy for pointing, tracking, and guiding involves two transformations.
First the transformation of target coordinates from celestial [α, δ] (or [Az,
El]) into an observed direction [Az, El] to point the telescope, accounting for:
coordinate systems, space motions, Earth motions, refraction, and other
non-instrumental terms.  Next the pointing kernel transforms observed [Az, El]
direction into mechanical encoder direction for the mount, via a “pointing
model” to account for: misalignment of the mount, tube flexure, instrument
flexure, etc., as well as user desired pointing origin, and offset from base
(e.g., for scanning).

Fully specifying a target’s location in the sky requires the following
information:

  * Coordinates – In [α, δ] or [Az, El].

  * Coordinate system – e.g., whether the [α, δ] is in FK4, FK5, or Topocentric
    apparent system.

  * Equinox – e.g., B1950.0 or J2000.  Before 1984, the equinox is usually in
    the B system, whereas on or after 1984, it is usually in the J system.  The
    default is to assume J2000, FK5 coordinate system.

  * Epoch – For objects that have proper motions, this is the time-zero from
    which proper motions are calculated.  The epoch is generally specified as a
    given year (e.g., 1964.25).  

  * Proper motion – There are various units for proper motion (arcsec yr-1,
    arcmin century-1, etc.).  By default proper motion is zero.

  * Parallax – Usually has units of [arcsec], and the default value is zero.

  * Radial velocity – Usually has units of [km/s], and the default value is zero.

Supported coordinate systems and the subsequent transformation flow are
described below.

*Coordinate Systems*

  The coordinate systems that will be supported are listed and described in
  Table 10‑14; the default will be ICRS:

+---------------------------+---------------------------------------------------------------------------+
|  Coordinate System        |  Description                                                              |
+===========================+===========================================================================+
| | FK4 [α, δ]              | | Often referred to as the B1950 (Besselian epoch 1950.0) coordinate      |
| |                         | | system, this mean equatorial system pre-dates the 1976 IAU resolutions. |
+---------------------------+---------------------------------------------------------------------------+
| | FK5 [α, δ]              | | Often referred to as the J2000 (Julian epoch 2000.0) coordinate system, |
| |                         | | this mean equatorial system post-dates the 1976 IAU resolutions.        |
+---------------------------+---------------------------------------------------------------------------+
| | ICRS [α, δ]             | | The International Celestial Reference System (ICRS) is the current      |
| |                         | | standard adopted by the International Astronomical Union, beginning     |
| |                         | | 1998. The origin is at the barycenter of the solar system with the axes |
| |                         | | fixed in space, and corresponds closely to FK5 J2000 to within ~30      |
| |                         | | mas for an object. [Default]                                            |
+---------------------------+---------------------------------------------------------------------------+
| | Topocentric             | | Mostly used for solar system targets when inputs come from solar system |
| | apparent [α, δ]         | | ephemeris.  programs where parallax (annual and diurnal), planetary     |
| |                         | | aberrations, and their motions (i.e., tracking rates in [α, δ]), would  |
| |                         | | already have been pre-computed.                                         |
+---------------------------+---------------------------------------------------------------------------+
| | Observed [Az, El]       | | Used when another program, external to the TCS, has made all the        |
| |                         | | transformations necessary.                                              |
+---------------------------+---------------------------------------------------------------------------+
| | Mount [Az, El]          | | Mostly used for engineering and calibration applications. The relation  |
| |                         | | between the Mount [Az, El] and observed [Az, El] is given by the        |
| |                         | | pointing model.                                                         |
+---------------------------+---------------------------------------------------------------------------+

*Celestial Coordinate System Transformation Flow*

  Given a coordinate [α, δ] in any celestial coordinate system by an astronomer,
  the TCS will convert it to FK5, J2000, and the current epoch, before producing
  the apparent [α, δ] used to aim the telescope. The transformation flow and the
  terms to correct for are given in the following
  :ref:`Figure<figure-transformation_flow>`. The TCS uses SLALIB20 to transform
  celestial coordinates into an apparent direction to point the telescope. New
  coordinate systems will be supported as interests and needs arise by upgrading
  or replacing SLALIB with another tool.

  .. _figure-transformation_flow:

  .. figure:: _static/tcs-transformation-flow.png

     Transformation Flow

  The above Figure shows the transformation flow taking coordinates from one
  reference frame to another, adapted from Wallace21. All input coordinates are
  eventually converted into FK5, J2000, current epoch, before final conversions
  to apparent [α, δ] to point the telescope.

*Transformation of Celestial System to Mount Encoder Positions*

  The :ref:`TCS/MCS Pointing Flow Figure (below)<figure-pointing_flow>` shows
  the steps taken to convert celestial coordinates [α, δ] into instrumental
  direction [Az, El] and rotator angles that the mount servo system needs to
  position the telescope. There are two flow directions, distinguished by how
  often the information needs to update: downward (slow, ≳20 Hz) and upward
  (fast, ≫20 Hz). The downward direction (upper half of Figure) first transforms
  target celestial coordinates into a corresponding line-of- sight [Az, El]
  coordinate, correcting for terms that are independent of the telescope mount.
  Those terms include: aberrations (annual and diurnal), light deflection,
  precession/nutation, Earth’s rotation, motion around the sun, parallax, and
  refraction.

  Next, additional errors factor in via pointing models (blue box, and see
  :ref:`Virtual Telescope Figure (below)<figure-virtual_telescope>`), such as
  mount non-perpendicularity and tube flexure. A pointing model consists of a
  set of coefficients that accounts for non-perpendicularity of the telescope
  axes, imperfections in alignment, flexure, or other mechanical imperfections.
  Lastly, to position the target coordinate at the user desired locationon the
  science detector (i.e., a slit or image pixel), it is necessary to offset the
  pointing origin to that location, which involves adjusting the rotator angle
  and position offset relative to the optical telescope axis. The above
  transformations result in the actual demand, angle [Az, El] for aiming the
  telescope mount.

  In contrast, upward transformations (lower half of Figure) take place at much
  higher frequencies (≫20 Hz), the purpose of which is to react to fast
  mechanical positioning demands, such as target scanning, tip-tilt guiding,
  wind perturbations, AO closed loop operations, image centroiding, and encoder
  errors.  The telescope Az/El servo system compares the mount demand (downward
  flow) versus that achieved (upward flow) to determine the appropriate actions
  for the mount.

  .. _figure-pointing_flow:

  .. figure:: _static/tcs-pointing-flow.png
     :align: center

     TCS/MCS Pointing Flow

  The TCS/MCS pointing flow, above, adapted from Wallace (2012), shows the
  transformation flow that takes target positions from the TCS (e.g., celestial
  coordinates [α, δ]) into mount encoder demands delivered to the MCS servo
  systems (in small box).  Arrows indicate the different pointing coordinates
  that users may specify and where they enter the flow.  Details of the pointing
  model, blue box, are elaborated in :ref:`the Figure
  below<figure-virtual_telescope>`.

*Virtual Telescopes and the Telescope Pointing Model*

  The concept and use of a Virtual Telescope was introduced earlier, as one of
  the three main components that make up :ref:`the Telescope Pointing
  Kernel<figure-tel_pointing_kernel>`.  To review, the purpose of the virtual
  telescope is to make all the necessary coordinate transformations and
  mechanical correction so the telescope can point accurately to a command
  position in a manner transparent to users.  The transformation flows are given
  in :ref:`the Pointing Flow Figure (above)<figure-pointing_flow>` and :ref:`the
  Virtual Telescope Figure (below)<figure-virtual_telescope>`.  The positional
  astronomy transformations from celestial coordinates into observed direction
  is well-established and will use SLALIB20.  Mechanical errors, misalignments,
  and flexure mapping/corrections will be implemented using TCSpk21 and
  diagnosed using TPpoint Software.

  Virtual telescopes with different focal positions can exist at the same time,
  which enables fast dithering, fast re-positioning to transfer a target image
  from one instrument to another and the ability to hold a sky target fixed at a
  specified position and orientation on an instrument even as the rotator turns.
  The virtual telescope generates mechanical demands (Az, El, rotator angle) to
  the mount encoders to reposition the telescope.  Finally, the virtual
  telescope allows for easy updates to the pointing model without having to
  modify the source code for the pointing kernel.

  Each focal station that has an instrument, including all wavefront sensors,
  engineering cameras, and science instruments, will have at least one virtual
  telescope and a pointing model. It is not unusual for an instrument to have
  multiple virtual telescopes depending on the purpose, such as for generating
  rotator and mount demands separately while the telescope is slewing.

  The pointing model in the virtual telescope accounts for mechanical or
  structural imperfections, such as telescope flexure, mount axis
  non-perpendicularity, encoder errors, instrument misalignment, encoder errors,
  etc. Its position in the flow of coordinate transformations is shown as blue
  boxes in :ref:`the Figure above (Pointing Flow)<figure-pointing_flow>`  and
  :ref:`Figure below (Virtual Telescope)<figure-virtual_telescope>`. The output
  of the virtual telescope, after accounting for the pointing model, is a
  trajectory for mount and rotator encoder demands which is delivered to the
  mount and rotator control servo.  Figure 10-22 shows the pointing data flow in
  natural seeing observing mode

  .. _figure-virtual_telescope:

  .. figure:: _static/tcs-virtual-telescope.png
     :align: center

     Virtual Telescope and Pointing Model

  The above Figure shows the "Virtual Telescope" and transformation terms that
  make up the pointing model; figure taken from Wallace20. The telescope
  pointing model (blue box) typically consists of 10-20 correction terms, here
  summarized into 7-terms: IA (roll), IB (pitch), VD (vertical deflection), CA
  (optical telescope axis/pitch non-perpendicularity), NP (roll/pitch
  non-perpendicularity), AW and AN (roll axis misalignments).