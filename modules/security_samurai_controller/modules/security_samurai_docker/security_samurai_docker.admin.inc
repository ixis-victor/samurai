<?php
/**
 * @file
 * security_samurai_docker.admin.inc
 * Administration funcions
 */

/**
 * Implements hook_form
 */
function security_samurai_docker_admin_config_form($form, &$form_state) {

  $form = array();

  // Max amount of docker containers
  $form['samurai_docker_max_containers'] = array(
    '#type' => 'select',
    '#title' => t('Max Docker containers'),
    '#description' => t('The max number of Docker containers that can be running at any one time.'),
    '#default_value' => variable_get('samurai_docker_max_containers', '3'),
    '#options' => range(1, 20),
  );

  $form = system_settings_form($form);

  return $form;
}
