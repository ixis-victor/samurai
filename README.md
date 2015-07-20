# TODO
- Important things
    - Make the theme responsive (theme is not completed in desktop yet).
- Moderately important things
    - Administrative users should be able to mark project types, or mark them to be ignored in automated security updates per site.
    - Theme the notification messages.
- Nice to have which probably won't get done.
    - Implement a search feature. What would need searching that doesn't isn't already < 2 clicks away?
    - Implement a secure_minor version to the security announcement. At the moment this is not possible - Drupal.org does not include a secure version number with it's security announcements. Only way I can think of doing this:
        - Upon a new security update being detected (on the day of announcement) take the latest secure minor from the update data for the project.
    - @see samurai_process_update_data Think of a better, more efficient way to do this.
- Meh:
    - Is samurai_get_site_update_data() necessary?

# Extra things to be done - after the TODO
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