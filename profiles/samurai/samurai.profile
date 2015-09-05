<?php
/**
 * @file
 * Enables modules and site configuration for the Samurai site installation.
 */

// Initalise the site_install_hooks
if (!function_exists('site_install_hooks_initialize')) {
  require_once('libraries/site_install_hooks/site_install_hooks.inc');
}
site_install_hooks_initialize('samurai');

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function samurai_form_install_configure_form_alter(&$form, $form_state) {

  // Pre-populate the site name with the profile name.
  $form['site_information']['site_name']['#default_value'] = 'Samurai';
}

/**
 * Implements hook_install_tasks()
 */
function samurai_install_tasks(&$install_state) {

  $task = array();

  // Require controller for profile install
  require_once(drupal_get_path('module', 'security_samurai_controller') . '/security_samurai_controller.profile.inc');

  // Interval configuration
  $task['samurai_interval_config'] = array(
    'display_name' => st('Samurai interval configuration'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'security_samurai_controller_interval_form',
  );

  // Require Docker for profile install
  require_once(drupal_get_path('module', 'security_samurai_docker') . '/security_samurai_docker.profile.inc');

  // Docker configuration
  $task['samurai_docker_config'] = array(
    'display_name' => st('Samurai environment configuration'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'security_samurai_docker_profile_config_form',
  );
  // Create the default Docker image
  $task['samurai_create_default_image'] = array(
    'display_name' => st('Create default image'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'security_samurai_docker_default_image_form',
  );

  // Require GitAPI for profile install
  require_once(drupal_get_path('module', 'security_samurai_gitapi') . '/security_samurai_gitapi.profile.inc');

  // GitAPI configuration
  $task['samurai_gitapi_config'] = array(
    'display_name' => st('Samurai GitAPI configuration'),
    'display' => TRUE,
    'type' => 'form',
    'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    'function' => 'security_samurai_gitapi_profile_config_form',
  );

  return $task;
}
