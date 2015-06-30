<?php
/**
 * @file
 * Administrative UI functions.
 *
 * These functions handle creating a new entity
 * entity type of 'client
 */

/**
 * Implements hook_form
 */
function client_form($form, &$form_state, $client = NULL) {
  
  $form = array();

  // The human readable name to give the client.
  $form['name'] = array(
    '#title' => t('Client name'),
    '#type' => 'textfield',
    '#default_value' => isset($client->name) ? $client->name : '',
    '#description' => t('The name of the client.'),
    '#required' => TRUE,
  );

  // The encryption key.
  $form['encryption_key'] = array(
    '#title' => t('Encryption key'),
    '#type' => 'textfield',
    '#default_value' => isset($client->encryption_key) ? $client->encryption_key : '',
    '#description' => t('The key provided by the client site.'),
    '#required' => TRUE,
  );

  // Check if the clients site_url is already an existing value.
  // If it isn't, then the client has not been submitted before and the site_url
  // will be extracted from the enryption_key
  if (isset($client->site_url)) {
    $form['site_url'] = array(
      '#title' => t('Site URL'),
      '#type' => 'textfield',
      '#default_value' => isset($client->site_url) ? $client->site_url : '',
      '#description' => t('The client site URL, decoded from the encryption key'),
      '#required' => TRUE,
    );
  }

  // Default form submit actions.
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($client->id) ? t('Update client') : t('Create client'),
    ),
  );

  // Return the form components.
  return $form;
}

/**
 * Implements hook_validate
 */
function client_form_validate($form, $form_state) {

  // The encryption key must be 32 or more characters long.
  if (strlen($form_state['values']['encryption_key']) < 32) {
    // Error handling.
    form_set_error('encryption_key', t('Please enter a valid encryption key.'));
  }
}

/**
 * Implements the default submit handler
 */
function client_form_submit($form, &$form_state) {

  // Decrypt the site_url from the encryption string.
  if (!isset($form_state['values']['site_url'])) {
    $encryptionsiteurl = $form_state['values']['encryption_key'];
    $site_url = substr($encryptionsiteurl, 32);
    $encryption_string = substr($encryptionsiteurl, 0, 32);
    $form_state['values']['encryption_key'] = $encryption_string;
    $form_state['values']['site_url'] = base64_decode($site_url);
  }
  // We need to check if the client is being updated or saved.
  if (!samurai_client_exists($form_state['values']['name'], $form_state['values']['site_url'])) {
    // Set the update_next_check to 0, so the updates are checked 
    // on the next cron execution.
    $form_state['values']['update_next_check'] = 0;
  }

  // Check that samurai can access the client site.
  if (samurai_check_site_access($form_state['values']['site_url'])) {

    // -- Save the entity.
    // -- Print notification messages.
    // -- Log to watchdoge.
    // -- Redirect to admin/client.
    $client = entity_ui_form_submit_build_entity($form, $form_state);
    $client->save();
    drupal_set_message(t('@name has been saved.', array('@name' => $client->name)));
    watchdog('Security samurai', t('@name has been saved.', array('@name' => $client->name)));
    $form_state['redirect'] = 'admin/client';

  } else {
    // Error handling.
    form_set_error('encryption_key', t('Samurai wasn\'t to establish a connection to the site.'));
  }
}

