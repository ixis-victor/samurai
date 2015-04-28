# TODO

- Important things
    - Finish setting up elyisa cron.
    - Finish the checking for general module updates.
    - Implement a function that will check samurai has access to the site before saving it. If samurai can't access the site - throw an error.
    - Update permissions for the new entity types e.g. Security announcement, Project.
    - Print an update list on the client page.
- Moderately important things
    - Utilise views.inc to make the views look and work nicer.
    - Administrative users should be able to mark project types, or mark them to be ignored in automated security updates.
    - The 'my account' page needs theming.
    - Theme the notification messages.
- Nice to have/do
    - Implement a search feature.
    - Combine $modules and $themes in @see _samurai_project_update_data()
    - Implement a secure_minor version to the security announcement. At the moment this is not possible - Drupal.org does not include a secure version number with it's security announcements.
    - @see samurai_process_update_data think of a better more efficient way to do this.

# THEN AUTOMATE ALL THE THINGS!