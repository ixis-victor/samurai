<?php
/**
 * @file
 * security_samurai_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function security_samurai_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'automated_jobs';
  $page->task = 'page';
  $page->admin_title = 'Automated jobs';
  $page->admin_description = '';
  $page->path = 'job';
  $page->access = array();
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Jobs',
    'name' => 'main-menu',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_automated_jobs__panel_context_96635844-012a-4db7-88f8-3e88e4f2ee45';
  $handler->task = 'page';
  $handler->subtask = 'automated_jobs';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'dc0b6c4b-8b68-43f3-9fb3-84080277c70a';
  $display->content = array();
  $display->panels = array();
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['automated_jobs'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'clients';
  $page->task = 'page';
  $page->admin_title = 'clients';
  $page->admin_description = '';
  $page->path = 'client';
  $page->access = array();
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Sites',
    'name' => 'main-menu',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_clients__panel_context_a9c6741f-f6dd-47a5-b1a4-58bcba56a7ea';
  $handler->task = 'page';
  $handler->subtask = 'clients';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Clients';
  $display->uuid = '1c5c992f-efbd-4b42-b482-543407db6bda';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-8dcabdfc-5454-46ce-b4a8-9b5202f19b2e';
    $pane->panel = 'left';
    $pane->type = 'views_panes';
    $pane->subtype = 'clients-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '8dcabdfc-5454-46ce-b4a8-9b5202f19b2e';
    $display->content['new-8dcabdfc-5454-46ce-b4a8-9b5202f19b2e'] = $pane;
    $display->panels['left'][0] = 'new-8dcabdfc-5454-46ce-b4a8-9b5202f19b2e';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['clients'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'dashboard';
  $page->task = 'page';
  $page->admin_title = 'Dashboard';
  $page->admin_description = '';
  $page->path = 'dashboard';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_dashboard__panel_context_aba8d883-94d2-4435-909c-45d37205b55b';
  $handler->task = 'page';
  $handler->subtask = 'dashboard';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '9859032d-b5e1-41f0-888f-53f1101a3119';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-30ce9858-292d-407a-a4e5-c53919ab16b1';
    $pane->panel = 'left';
    $pane->type = 'views_panes';
    $pane->subtype = 'clients-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '30ce9858-292d-407a-a4e5-c53919ab16b1';
    $display->content['new-30ce9858-292d-407a-a4e5-c53919ab16b1'] = $pane;
    $display->panels['left'][0] = 'new-30ce9858-292d-407a-a4e5-c53919ab16b1';
    $pane = new stdClass();
    $pane->pid = 'new-7de5e9c2-1307-410d-a846-7811e5c35158';
    $pane->panel = 'right';
    $pane->type = 'views_panes';
    $pane->subtype = 'security_announcements-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '7de5e9c2-1307-410d-a846-7811e5c35158';
    $display->content['new-7de5e9c2-1307-410d-a846-7811e5c35158'] = $pane;
    $display->panels['right'][0] = 'new-7de5e9c2-1307-410d-a846-7811e5c35158';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-30ce9858-292d-407a-a4e5-c53919ab16b1';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['dashboard'] = $page;

  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'security_announcement';
  $page->task = 'page';
  $page->admin_title = 'Security announcement';
  $page->admin_description = '';
  $page->path = 'security-announcement';
  $page->access = array();
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Security announcements',
    'name' => 'main-menu',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_security_announcement__panel_context_85b31cf4-f8de-4299-8a03-6a54c08ade22';
  $handler->task = 'page';
  $handler->subtask = 'security_announcement';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'twocol_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Security announcements';
  $display->uuid = '99ee80ee-8931-40c2-858a-2699c43e29da';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-76576062-e8c4-4b7a-b211-aeb186cd8cd3';
    $pane->panel = 'left';
    $pane->type = 'views_panes';
    $pane->subtype = 'security_announcements-panel_pane_2';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '76576062-e8c4-4b7a-b211-aeb186cd8cd3';
    $display->content['new-76576062-e8c4-4b7a-b211-aeb186cd8cd3'] = $pane;
    $display->panels['left'][0] = 'new-76576062-e8c4-4b7a-b211-aeb186cd8cd3';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['security_announcement'] = $page;

  return $pages;

}
