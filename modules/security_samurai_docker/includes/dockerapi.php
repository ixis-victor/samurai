<?php

class Docker {

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
  public static function execute_command($container_id, $command) {

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
  public static function create_image($image_name, $dockerfile_location) {
    
    // Construct the command.
    $command = 'docker build -t ' . $image_name . ' ' . $dockerfile_location;

    // - Escape shell metacharacters.
    // - Execute the command.
    $command = escapeshellcmd($command);
    $result = system($command);

    // The result returned is the image ID.
    $this->image_id = $result;
  }

  /**
   * Delete image
   * Deletes an image.
   *
   * @param string $image_name The name of the image to delete.
   */
  public static function delete_image($image_name) {

    // Need to make sure all containers using this image are stopped or deleted.
    // Structure
    // docker rmi {image_name}
  }

  /**
   * Create container
   * Create a new container.
   *
   * @param string $image_name The name of the image to create a container from.
   * @param array $assigned_ports The assigned ports e.g. 80 => 3000.
   */
  public static function create_container($image_name, $assigned_ports) {

    // Structure
    // docker run -d -i -t -p {assigned_ports} {image_name}
    // returns container hash - this will need returning and info added to the container entity
  }

  /**
   * Delete container
   * Delete a docker container. Cannot be recovered.
   *
   * @param string $container_id The container ID.
   */
  public static function delete_container($container_id) {

    // Structure
    // docker rm {container_id}
  }

  /**
   * Stop container
   * Stop a docker container from running. State is saved.
   *
   * @param string $container_id The container ID.
   */
  public static function stop_container($container_id) {

    // Structure
    // docker stop {container_id}
  }

  /**
   * Start container
   * Start docker container.
   *
   * @param string $container_id The container ID.
   */
  public static function start_container($container_id) {

    // Structure
    // docker start {container_id}
  }

  /**
   * Export a container
   * Export a docker container for download 
   *
   * @param string $container_id The container ID.
   * @param string $output_file_location The location to save the container.
   */
  public static function export_container($container_id, $output_file_location) {

    // Structure
    // docker export {container_id} > {output_file_location}
  }
}
