# TODO

- Important things
    - Make the theme responsive (theme is not completed in desktop yet).
    - Finish setting up elyisa cron.
    - Finish the checking for general module updates.
    - Implement a function that will check samurai has access to the site before saving it. If samurai can't access the site - throw an error.
    - Update permissions for the new entity types e.g. Security announcement, Project.
    - Add a core version number to the site entity type so it can be added as a filter in the site view page.
- Moderately important things
    - Administrative users should be able to mark project types, or mark them to be ignored in automated security updates per site.
    - The 'my account' page needs theming.
    - Theme the notification messages.
- Nice to have which probably won't get done.
    - Implement a search feature.
    - Combine $modules and $themes in @see _samurai_project_update_data()
    - Implement a secure_minor version to the security announcement. At the moment this is not possible - Drupal.org does not include a secure version number with it's security announcements. Only way I can think of doing this with the least amount of bugs:
        - Upon a new security update being detected (on the day of announcement) take the latest secure minor from the update data for the project.
    - @see samurai_process_update_data Think of a better, more efficient way to do this.
- Meh:
    - Is samurai_get_site_update_data() necessary?

# Start on auti
- Send an email to all subscribed users when a security update is available with a list of affected sites.
- Docker
    - The module controls the amount of containers that are running.
    - An ssh key would have to be generated and added automatically.
        - Codebase API, GitHub API
            - possible.
        - This is were custom modules for github, codebase and bitbucket come in.
    - Used for spinning up an environment to run the tests on.
    - Run a script to clone the repo, run the update script, run the tests and commit up to the repo.
- Codeception
    - Used for running tests on docker environments.
    - Downloads the tests from a Git repo?
- Drush aliases
    - Use to check for updates 