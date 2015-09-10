# Security samurai: Automated Drupal security updates
---

### What happens when a security update is released?
Usually this process is quite slow to do manually, but with Samurai, the update can be done in under 5 minutes. Here's how:
1. TODO  -_-

### Requirements
 - Docker >= 1.8.1 (version)

### Installation
This installation assumes you already have a server set up that can run Drupal. More info: https://www.drupal.org/requirements
1. First off you need to install the latest version of Docker (http://docs.docker.com/linux/step_one/)
2. After installing Docker you will want to create a new user group on the server called 'docker': ``` $ groupadd docker ```
3. Add the web server user to the 'docker' group: ``` $ usermod www-data -G docker ```
4. Restart the apache and docker services for the group changes to take effect: ``` $ service docker restart && service apache2 restart```
5. Install Drupal as normal by visiting <YOUR_SITE_ADDRESS>/install.php

##### Setting up drush aliases
If you use drusb aliases (highly recommended), the alias files go in the ``` /etc/drush ``` directory where they can be access by the webuser.
Samurai takes advantage of drush aliases by allowing you to create new test environments on the fly for your own use.

##### Git
When performing security updates Samurai currently only supports Git Flow (a workflow for Git: http://nvie.com/posts/a-successful-git-branching-model/)
