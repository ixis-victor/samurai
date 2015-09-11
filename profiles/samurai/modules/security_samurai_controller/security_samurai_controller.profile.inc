<?php
/**
 * @file
 * security_samurai_controller.profile.inc
 * Profile install functions
 */

/**
 * Retuns a form for configuring update checking intervals
 */
function security_samurai_controller_interval_form($form, &$form_state) {

  $form = array();

  // General information
  $form['general_information'] = array(
    '#type' => 'markup',
    '#markup' => t('Each of these values can be reconfigured in the administration interface'),
  );
  // Site configuration wrapper
  $form['site_wrapper'] = array(
    '#type' => 'fieldset',
    '#title' => t('Interval configuration'),
    '#description' => t('Configure values for regarding update checking intervals'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Check the site for new updates.
  $form['site_wrapper']['site_check_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to scan a site'),
    '#description' => t('Scan a site at an interval for module updates. Recommended 2 days.'),
    '#options' => security_samurai_controller_get_interval_options(),
    '#required' => TRUE,
  );
  // Check projects for new updates
  $form['site_wrapper']['module_check_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to check for general module updates'),
    '#description' => t('Interval to check a module for a new version, this is used to indicate you when a module becomes out of date. Recommended 1 day.'),
    '#options' => security_samurai_controller_get_interval_options(),
    '#required' => TRUE,
  );
  // Check for new security updates
  $form['site_wrapper']['security_interval'] = array(
    '#type' => 'select',
    '#title' => t('Interval to check for security updates'),
    '#description' => t('Interval to check for a security update, upon a security announcement affecting an attached site, samurai will initiate the update process. Recommended 5 mins'),
    '#options' => security_samurai_controller_get_interval_options(),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save and continue'),
  );

  return $form;
}

/**
 * Form validate handler
 * @see security_samurai_controller_interval_form
 */
function security_samurai_controller_interval_form_validate($form, $form_state) {

  //
}

/**
 * Form submit handler
 * @see security_samurai_controller_interval_form
 */
function security_samurai_controller_interval_form_submit($form, $form_state) {

  // Retrieve the strings of the security_samurai_controller_get_interval_options
  $options = security_samurai_controller_get_interval_options();
  $site_check_interval = $options[$form_state['values']['site_check_interval']];
  $module_check_interval = $options[$form_state['values']['module_check_interval']];
  $security_interval = $options[$form_state['values']['security_interval']];

  // Set the variables
  variable_set('security_samurai_controller_site_check_interval', $site_check_interval);
  variable_set('security_samurai_controller_module_check_interval', $module_check_interval);
  variable_set('security_samurai_controller_sa_check_interval', $security_interval);
}

/**
 * Helper function, returns an array of intervals
 * @see security_samurai_controller_interval_form()
 */
function security_samurai_controller_get_interval_options() {

  // Return the array of intervals
  return array(
    0 => '5 minutes',
    1 => '30 minutes',
    2 => '1 hour',
    3 => '3 hours',
    4 => '6 hours',
    5 => '12 hours',
    6 => '1 day',
    7 => '2 days',
    8 => '4 days',
    9 => '1 week',
    10 => '1 month',
  );
}
