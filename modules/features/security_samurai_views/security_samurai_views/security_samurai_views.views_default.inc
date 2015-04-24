<?php
/**
 * @file
 * security_samurai_views.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function security_samurai_views_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'clients';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'securitysamurai_client';
  $view->human_name = 'Clients';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Client overview';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Client: Client ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'securitysamurai_client';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Client: Label */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'securitysamurai_client';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Site name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['path'] = 'client/[id]';
  /* Field: Client: Update_count */
  $handler->display->display_options['fields']['update_count']['id'] = 'update_count';
  $handler->display->display_options['fields']['update_count']['table'] = 'securitysamurai_client';
  $handler->display->display_options['fields']['update_count']['field'] = 'update_count';
  $handler->display->display_options['fields']['update_count']['label'] = 'Available security updates';
  /* Sort criterion: Client: Update_count */
  $handler->display->display_options['sorts']['update_count']['id'] = 'update_count';
  $handler->display->display_options['sorts']['update_count']['table'] = 'securitysamurai_client';
  $handler->display->display_options['sorts']['update_count']['field'] = 'update_count';
  $handler->display->display_options['sorts']['update_count']['order'] = 'DESC';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $export['clients'] = $view;

  $view = new view();
  $view->name = 'security_announcements';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'securitysamurai_sa';
  $view->human_name = 'Security announcements';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recent security announcements';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '6';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Security announcement: Security announcement ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Security announcement: Label */
  $handler->display->display_options['fields']['sa_id']['id'] = 'sa_id';
  $handler->display->display_options['fields']['sa_id']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['sa_id']['field'] = 'sa_id';
  $handler->display->display_options['fields']['sa_id']['label'] = '';
  $handler->display->display_options['fields']['sa_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['sa_id']['alter']['path'] = 'security-announcement/[id]';
  $handler->display->display_options['fields']['sa_id']['element_label_colon'] = FALSE;
  /* Field: Security announcement: Project_name */
  $handler->display->display_options['fields']['project_name']['id'] = 'project_name';
  $handler->display->display_options['fields']['project_name']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['project_name']['field'] = 'project_name';
  $handler->display->display_options['fields']['project_name']['label'] = 'Affected project';
  /* Field: Security announcement: Risk_level */
  $handler->display->display_options['fields']['risk_level']['id'] = 'risk_level';
  $handler->display->display_options['fields']['risk_level']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['fields']['risk_level']['field'] = 'risk_level';
  $handler->display->display_options['fields']['risk_level']['label'] = 'Risk level';
  /* Sort criterion: Security announcement: Date */
  $handler->display->display_options['sorts']['date']['id'] = 'date';
  $handler->display->display_options['sorts']['date']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['sorts']['date']['field'] = 'date';
  $handler->display->display_options['sorts']['date']['order'] = 'DESC';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $export['security_announcements'] = $view;

  return $export;
}
