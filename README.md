# Globally Consistent Multi-People Tracking using Motion Patterns
Created by Andrii Maksai at CVLAB, EPFL.

### Introduction

This is an approach for simultaneous tracking and learning the motion patterns
of the people on the scene.

### License

This work is released under the MIT License (refer to the LICENSE file for details).

### Citation

This code accompanies paper "Globally Consistent Multi-People Tracking using
Motion Patterns".  Link to the arxiv submission to be added soon.

### Installation

Requirements:

1. [Gurobi](www.gurobi.com/), free with academic license.  $GUROBI\_HOME
environment variable should exist and gurobi\_cl tool should be available.

2. g++

To install, `make`. Run `bin\test` to ensure that everything should work on
your system. It has been tested on some versions of OS X and Ubuntu.

### Data

   **Towncentre** dataset from 2D MOT benchmark split into 3 1-minute segments
   accompanies the submission. Original frames are available from
   https://motchallenge.net/data/2D_MOT_2015/, and ground truth is available
   from
   http://www.robots.ox.ac.uk/~lav/Research/Projects/2009bbenfold_headpose/project.html

### Usage


1. To reproduce the results reported in the paper (approximately, as the
   presented version does not optimize hyperparameters using cross-validation),
   run `bin\eval`.

2. There are 3 possible modes of operation: **a:** learning patterns from the
   ground truth, **b:** improving tracking results of other method using the
   learned patterns, and **c:** learning the patterns and improving the
   tracking in unsupervised fashion without the ground truth.

   You can run the examples as follows:
   `bin/ptrack examples/training.cfg`
   `bin/ptrack examples/testing.cfg`
   `bin/ptrack examples/unsupervised.cfg`

   Note that you should run training before running testing. The .cfg files in
   the examples directory explain in comments which entries should be present
   in .cfg files for each of the 3 tasks. To run it on your data, you need to
   create your own .cfg file and run bin/ptrack on it.

### Output format

1. The tracking results are saved in the MOTChallenge format - see
https://motchallenge.net/data/2D_MOT_2015 for more details.
2. The patterns are saved in the format \<Number of points on the pattern
   centerline N\> \<Width of the pattern\> \<X_1\> \<Y_1\> ... \<X_N\> \<Y_N\>,
   or a keyword "NoPattern" for special pattern for anomalous behaviours.
3. The full result file starts with the number of patterns and the number of
   trajectories. Then follow the description of all patterns in the format
   described above. Then follow the description of all trajectories in the
   following format: \<Number of points on the trajectory M\> \<Number of
   assigned pattern\> \<Frame #1\> \<X_1\> \<Y_1\> ... \<Frame #M\> \<X_M\>
   \<Y_M\>.

### Contact

Please contact andrii dot maksai at epfl dot ch for any related queries.
