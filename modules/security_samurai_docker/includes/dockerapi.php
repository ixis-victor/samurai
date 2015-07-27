<?php

class Docker {

  // General variables
  public $dockerfile_location;

  // Container variables
  public $container_id;
  public $container_image_name;
  public $assigned_ports;
  public $status;

  // Image variables
  public $image_name;
  public $image_id;
  public $image_os;

  /**
   * Execute command
   * Execute a command on a docker container
   *
   * @param string $container_id The container ID.
   * @param string $command The command to execute
   */
  public static function execute_command($container_id = NULL, $command) {

    if (is_null($container_id) && !is_null($this->container_id)) {
      // If the container_id is NULL, get the class' value of the variable.
    }

    // Construct the command.
    $command = 'docker exec ' . $container_id . ' ' . $command;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    $result = system($command);

    // Return the result.
    return $result;
  }

  /**
   * Create image
   * Builds a new image for creating containers with.
   *
   * @param string $image_name The name of the image to create.
   * @param string $dockerfile_location The absolute location to the dockerfile.
   */
  public static function create_image($image_name, $dockerfile_location = NULL) {

    if (is_null($dockerfile_location) && !is_null($this->dockerfile_location)) {
      // If the dockerfile_location is NULL, get the class' value of the variable
      $dockerfile_location = $this->dockerfile_location;
    }
    
    // Construct the command.
    $command = 'docker build -t ' . $image_name . ' ' . $dockerfile_location;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    $result = system($command);

    // The result returned is the image ID.
    $this->image_id = $result;

    $this->dockerfile_location = $dockerfile_location;
  }

  /**
   * Delete image
   * Deletes an image.
   *
   * @param string $image_name The name of the image to delete.
   */
  public static function delete_image($image_name = NULL) {

    if (is_null($image_name) && !is_null($this->image_name)) {
      // If  the image_name is NULL, get the class' value of the variable.
      $image_name = $this->image_name;
    }

    // Construct the command.
    $command = 'docker rmi ' . $image_name;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    system($command);

    // No return output for this function.
  }

  /**
   * Create container
   * Create a new container.
   *
   * @param string $image_name The name of the image to create a container from.
   * @param array $assigned_ports The assigned ports e.g. 80 => 3000.
   */
  public static function create_container($image_name = NULL, $assigned_ports = NULL) {
    
    if (is_null($image_name) && !is_null($this->image_name)) {
      // If the image_name is NULL, get the class' value of the variable.
      $image_name = $this->image_name;
    }

    if (is_null($assigned_ports) && !is_null($this->assigned_ports)) {
      // If the assigned_ports is NULL, get the class' value of the variable.
      $assigned_ports = $this->assigned_ports;
    }

    // Construct the command.
    $command = 'docker run -d -i -t -p ' . $assigned_ports . ' ' . $image_name;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    $result = system($command);

    // The result returned is the container ID.
    $this->image_name = $image_name;
    $this->assigned_ports = $assigned_ports;
    $this->container_id = $result;
  }

  /**
   * Delete container
   * Delete a docker container. Cannot be recovered.
   *
   * @param string $container_id The container ID.
   */
  public static function delete_container($container_id = NULL) {

    if (is_null($container_id) && !is_null($this->container_id)) {
      // If container_id is NULL, get the class' value of the variable.
      $container_id = $this->container_id;
    }

    // Construct the command
    $command = 'docker rm ' . $container_id;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    system($command);

    // No return output for this function.
  }

  /**
   * Stop container
   * Stop a docker container from running. State is saved.
   *
   * @param string $container_id The container ID.
   */
  public static function stop_container($container_id = NULL) {

    // Structure
    // docker stop {container_id}
    // $command

    if (is_null($container_id) && !is_null($this->container_id)) {
      // If container_id is NULL, get the class' value of the variable.
      $container_id = $this->container_id;
    }

    // Construct the command.
    $command = 'docker stop ' . $container_id;


    // - Escape shell metascharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    system($command);

    // No return output for this function.

    // Set the container status variable to 0 (OFF).
    $this->status = 0;
  }

  /**
   * Start container
   * Start docker container.
   *
   * @param string $container_id The container ID.
   */
  public static function start_container($container_id = NULL) {

    if (is_null($container_id) && !is_null($this->container_id)) {
      // If container_id is NULL, get the class' value of the variable.
      $container_id = $this->container_id;
    }

    // Construct the command.
    $command = 'docker start ' . $container_id;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    system($command);

    // No return output for this function.

    // Set the container status variable to 1 (ON).
    $this->status = 1;
  }

  /**
   * Export a container
   * Export a docker container for download 
   *
   * @param string $container_id The container ID.
   * @param string $output_file_location The location to save the container.
   */
  public static function export_container($container_id = NULL, $output_file_location) {

    // Structure
    // docker export {container_id} > {output_file_location}

    if (is_null($container_id) && !is_null($this->container_id)) {
      // If container_id is NULL, get the class' value of the variable.
      $container_id = $this->container_id;
    }

    // Construct the command.
    $command = 'docker export ' . $container_id . ' > ' . $output_file_location;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    system($command);

    // No return output for this function
  }
}
