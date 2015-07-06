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
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
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
  /* Footer: Global: Unfiltered text */
  $handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['content'] = '<a href="/client">See more</a>';
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
  /* Filter criterion: Client: Label */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'securitysamurai_client';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Site name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;

  /* Display: Content pane 2 */
  $handler = $view->new_display('panel_pane', 'Content pane 2', 'panel_pane_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Clients';
  $handler->display->display_options['defaults']['footer'] = FALSE;
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
  $handler->display->display_options['title'] = 'Security announcements';
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
  /* Footer: Global: Unfiltered text */
  $handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['footer']['area_text_custom']['content'] = '<a href="/security-announcement">See more</a>';
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
  $handler->display->display_options['fields']['risk_level']['label'] = '';
  $handler->display->display_options['fields']['risk_level']['element_label_colon'] = FALSE;
  /* Sort criterion: Security announcement: Date */
  $handler->display->display_options['sorts']['date']['id'] = 'date';
  $handler->display->display_options['sorts']['date']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['sorts']['date']['field'] = 'date';
  $handler->display->display_options['sorts']['date']['order'] = 'DESC';
  /* Filter criterion: Security announcement: Risk_level */
  $handler->display->display_options['filters']['risk_level']['id'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['table'] = 'securitysamurai_sa';
  $handler->display->display_options['filters']['risk_level']['field'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['operator'] = 'contains';
  $handler->display->display_options['filters']['risk_level']['exposed'] = TRUE;
  $handler->display->display_options['filters']['risk_level']['expose']['operator_id'] = 'risk_level_op';
  $handler->display->display_options['filters']['risk_level']['expose']['label'] = 'Risk level';
  $handler->display->display_options['filters']['risk_level']['expose']['operator'] = 'risk_level_op';
  $handler->display->display_options['filters']['risk_level']['expose']['identifier'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['risk_level']['group_info']['label'] = 'Risk_level';
  $handler->display->display_options['filters']['risk_level']['group_info']['identifier'] = 'risk_level';
  $handler->display->display_options['filters']['risk_level']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['risk_level']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;

  /* Display: Content pane 2 */
  $handler = $view->new_display('panel_pane', 'Content pane 2', 'panel_pane_2');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '8';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $export['security_announcements'] = $view;

  return $export;
}
