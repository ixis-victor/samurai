#
# W.I.P
#

# Enable the contrib modules:
drush en -y ctools
drush en -y strongarm
drush en -y features
drush en -y views
drush en -y views_ui
drush en -y views_content
drush en -y pace
drush en -y libraries
drush en -y fontawesome
drush en -y panels
drush en -y page_manager
drush en -y elysia_cron
drush en -y entity
drush en -y require_login
drush en -y fontawesome
drush en -y jquery_update
drush en -y module_filter

# Enable the security samurai modules:
drush en -y security_samurai_base

# Disable select modules:
drush dis -y overlay

# Revert all features:
drush fra -y

# Remove the Home menu item:
drush sql-query "DELETE FROM `menu_links` WHERE menu_name='main-menu' AND link_path='<front>' AND link_title='Home';" 
