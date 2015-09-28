<?php
/**
 * @file
 * security_samurai_deploy.admin.inc
 * Administration functions
 */

/**
 * Returns the form array for configuring values
 * required by
 */
function security_samurai_deploy_admin_config_form($form, &$form_state) {

  $form = array();

  // Fieldset wrapper for the deployhq configurable
  // elements
  $form['deployhq_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('DeployHQ API'),
    '#description' => t('Manage configurable values for the DeployHQ API'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  // Domain prefix
  $form['deployhq_settings']['security_samurai_deploy_deployhq_domain_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain prefix'),
    '#description' => t('The domain prefix follows this pattern: <pre>https://DOMAIN_PREFIX.deployhq.com</pre>'),
    '#default_value' => variable_get('security_samurai_deploy_deployhq_domain_prefix', NULL),
  );
  // Username
  $form['deployhq_settings']['security_samurai_deploy_deployhq_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('The username from your ATech account'),
    '#default_value' => variable_get('security_samurai_deploy_deployhq_username', NULL),
  );
  // API key
  $form['deployhq_settings']['security_samurai_deploy_deployhq_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('The API key for the domain, usually found in the account profile'),
    '#default_value' => variable_get('security_samurai_deploy_deployhq_api_key', NULL),
  );

  return system_settings_form($form);
}
