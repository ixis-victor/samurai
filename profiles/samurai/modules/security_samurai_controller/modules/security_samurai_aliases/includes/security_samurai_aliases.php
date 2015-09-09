<?php
/**
 * @file
 * security_samurai_aliases.php
 *
 * Functions for executing drush commands
 */

class SamuraiAliases {

  /**
   * Returns all aliases in an clean array
   *
   * @param bool $exclude_self By default set to TRUE
   * The aliases '@self' and '@none' will be ommitted from the returned
   * array.
   */
  public function get_all_aliases($exclude_self = TRUE) {

    // Call invoke process to execute 'drush @self sa'
    $site_aliases = $this->invoke_process('', 'sa');

    // - Reassign variable for management
    $output = $site_aliases['output'];
    foreach ($output as $key => $value) {
      if ($exclude_self) {
        if ($value == '@none' || $value == '@self') {
          // Remove the value
          unset($output[$key]);
        }
      }
    }

    return $output;
  }

  /**
   * Invoke an external drush command process
   * All output is returned
   *
   * @param string $alias The site alias
   * @param string $command The command to execute on the site alias
   * @param string $args The EOL arguments
   */
  public function invoke_process($alias, $command, $args = NULL) {

    // Scope variables
    $build_command = NULL;

    // Only allow 'safe' commands to be executed
    switch ($command) {
      case 'sa':
        // drush @alias sa
        $build_command = 'drush ' . $alias . ' sa';
        break;
      case 'sql-dump':
        // drush @alias sql-dump > /tmp/blah.sql
        $build_command = 'drush ' . $alias . ' sql-dump ' . $args;
        break;
      case 'uli':
        // drush @alias uli
        // drush @alias uli USERNAME
        break;
    }

    // Check build_command isn't null before attempting
    // to execute it
    if (!is_null($build_command)) {

      // Execute build_command and return the output
      return $this->execute($build_command);
    } else {

      // Return FALSE
      return FALSE;
    }
  }

  /**
   * Execute an external command
   *
   * @param string $command The command to execute
   */
  protected function execute($command) {

    // - Append 2>&1 on to the end of the command
    // - Execute the command
    $command = $command . ' 2>&1';
    $result = exec($command, $output);

    // Return the array of output and result
    // - The result is the last line of output
    // - The output is an array of every line of output
    //   from the command
    return array(
      'output' => $output,
      'result' => $result
    );
  }
}
