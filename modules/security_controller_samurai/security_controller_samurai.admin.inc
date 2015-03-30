<?php
/**
 * @file
 * Administrative UI functions.
 */

// include(SAMURAI_MODULE_PATH . '/includes/samurai.inc');

/**
 * Implements hook_form
 */
function client_form($form, &$form_state, $client = NULL) {
  $form = array();

  $form['name'] = array(
    '#title' => t('Client name'),
    '#type' => 'textfield',
    '#default_value' => isset($client->name) ? $client->name : '',
    '#description' => t('The name of the client.'),
    '#required' => TRUE,
  );

  $form['encryption_key'] = array(
    '#title' => t('Encryption key'),
    '#type' => 'textfield',
    '#default_value' => isset($client->encryption_key) ? $client->encryption_key : '',
    '#description' => t('The key provided by the client site.'),
    '#required' => TRUE,
  );

  if (isset($client->site_url)) {
    $form['site_url'] = array(
      '#title' => t('Site URL'),
      '#type' => 'textfield',
      '#default_value' => isset($client->site_url) ? $client->site_url : '',
      '#description' => t('The client site URL, decoded from the encryption key'),
      '#required' => TRUE,
    );
  }

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => isset($client->id) ? t('Update client') : t('Create client'),
    ),
  );

  return $form;
}

/**
 * Implements hook_validate
 */
function client_form_validate($form, $form_state) {
  // The encryption key must be 32 or more characters long.
  if (strlen($form_state['values']['encryption_key']) < 32) {
    // Return error message.
    form_set_error('encryption_key', t('Please enter a valid encryption key.'));
  }
  // Check if the site_url has been set in the form.
  if (isset($form_state['values']['site_url'])) {
    // Check that samurai can access the samurai report page on the client site.
    // if (!_security_controller_check_site_access($form_state['values']['site_url'])) {
    //   // Return error if it can't access the report page.
    //   form_set_error('encryption_key', t('Cannot access the encrypted site'))
    // }
  } else {
    // site_url is empty. Take it from the encryption_key
    
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
  // Get the update data from the site.
  $update_data = _samurai_get_update_data($form_state['values']['site_url']);

  // Decrypt the update data and add the modules to the module checker table.
  $encrypted_string = drupal_json_decode($update_data);
  $decrypted_data = drupal_json_decode(trim(_samurai_decrypt_update_data($encrypted_string['encrypted'], $form_state['values']['encryption_key'])));
  $processed_data = drupal_json_encode(_samurai_process_update_data($decrypted_data));

  // Set a update_data and update_last_check form value.
  $form_state['values']['site_update_data'] = $processed_data;
  $form_state['values']['update_last_check'] = REQUEST_TIME;

  // Save the entity
  $client = entity_ui_form_submit_build_entity($form, $form_state);
  $client->save();

  // Messages
  drupal_set_message(t('@name has been saved.', array('@name' => $client->name)));
  // Log the client creation to watchdog.
  // @todo

  // Redirect to admin/client
  $form_state['redirect'] = 'admin/client';
}

