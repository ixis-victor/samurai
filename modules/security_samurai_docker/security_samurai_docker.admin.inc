<?php
/**
 * @file
 * security_samurai_docker.admin.inc
 * Administration funcions
 */

/**
 * Implements hook_form
 */
function security_samurai_docker_admin_config_form($form, &$form_state, $client = NULL) {

  $form = array();

  // Max amount of docker containers
  $form['samurai_docker_max_containers'] = array(
    '#title' => t('Max Docker containers'),
    '#description' => t('The max number of Docker containers that can be running at any one time.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('samurai_docker_max_containers', '3'),
  );

  $form = system_settings_form($form);

  return $form;
}
