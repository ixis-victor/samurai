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

  // Max number of active docker containers
  $form['samurai_docker_max_active_containers'] = array(
    '#type' => 'select',
    '#title' => t('Maximum active Docker containers'),
    '#description' => t('The max number of Docker containers that can be running at any one time.'),
    '#default_value' => variable_get('samurai_docker_max_active_containers', '3'),
    '#options' => range(1, 20),
  );
  // Max number of inactive docker containers
  $form['samurai_docker_max_inactive_containers'] = array(
    '#type' => 'select',
    '#title' => t('Maximum inactive Docker containers'),
    '#description' => t('The max number of Docker containers that can be left inactive on the system.'),
    '#default_value' => variable_get('samurai_docker_max_inactive_containers', '10'),
    '#options' => range(1, 100),
  );
  $form = system_settings_form($form);

  return $form;
}
