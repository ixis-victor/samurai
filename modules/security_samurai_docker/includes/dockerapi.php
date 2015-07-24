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
   */
  public function execute_command($container_id = NULL) {

    if (is_null($container_id)) {
      // Structure
      // docker exec {container_id} {command}
    } else {
      $this->container_id = $container_id;
    }
  }

  /**
   * Create image
   * Builds a new image for creating containers with.
   *
   * @param string $image_name The name of the image to create.
   * @param string $dockerfile_location The absolute location to the dockerfile.
   */
  public function create_image($image_name, $dockerfile_location) {
    
    // Structure
    // docker build -t {image_name} {dockerfile_location}
  }

  /**
   * Create container
   * Create a new container.
   *
   * @param string $image_name The name of the image to create a container from.
   * @param array $assigned_ports The assigned ports e.g. 80 => 3000.
   */
  public function create_container($image_name, $assigned_ports) {

    // Structure
    // docker run -d -i -t -p {assigned_ports} {image_name}
    // returns container hash
  }

  /**
   * Stop container
   * Stop a docker container from running. State is saved. 
   */
  public function stop_container($container_id) {

  }

  /**
   * Start container
   * Start docker container.
   */
  function start_container($container_id) {
    
  }
}
