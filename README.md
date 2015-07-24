
## Version 0.1.1

## W.I.P

In it's current state, Samurai has basic security update checking and site management.

### Installation

1. Create the sites directory:<br />
   ```
   $ mkdir sites
   ```
2. Change directory and clone the repo:<br />
   ```
   $ cd sites && git clone https://github.com/jkswoods/samurai.git
   ```
3. Run the make file with drush:<br />
   ```
   $ cd ../ && drush make sites/samurai/samurai.make -y
   ```
4. Run the Drupal installation with the standard profile.
5. Connect to your Virtual machine, and run the samurai.sh script in the ```samurai``` directory:<br/ >
   ```
   $ cd sites/samurai && ./samurai.sh
   ```


