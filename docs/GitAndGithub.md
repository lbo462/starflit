# How to work with git and GitHub

You need to have `git` installed on your computer (Linux is preferred but `git bash` for Windows works just fine).

Clone the repo with `git clone git@github.com:lbo462/starflit.git`, 
after having a clean [SSH-key access set-up](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)

The other useful and only commands you'll ever need for this project are:
- `git add -v .` to stage your local changes
- `git commit -m "Your message"` to commit these changes
- `git push origin <your-branch>` to push your commit to GitHub
- `git fetch origin -v` to fetch changes and new branches from GitHub
- `git checkout <branch>` to change the current branch
- `git rebase master -i` to rebase your branch commits on the commits added to `master`

And these ones might be of use:
- `git status` the see a global view of changed files
- `git diff` to see in details what changed
- `git checkout .` to reset your changes to the previous commit. __To use carefully__ 

Here's the standard working process:

__Don't push to__ `master`.
Instead ...

- Create an issue at [https://github.com/lbo462/starflit/issues](https://github.com/lbo462/starflit/issues)
and describe what needs to be done.

- On this issue, create a branch (checkout locally), at the bottom right of the page (under the Development section).

- From now on, `git fetch origin -v` to fetch the created branch and `git checkout <branch>` to work on the given branch.
You can freely make commits and push to that branch to implement what's needed.

- From that issue and once you made changes, create a pull request at 
[https://github.com/lbo462/starflit/pulls](https://github.com/lbo462/starflit/pulls)
and select the branch created for the issue.

- Once your work is done, assign someone to review your work and approve your changes.

- Once approved, you can __squash and merge__ your branch via the pull request you created.

- A good practice is to delete the merged branch once the pull request is accepted, and can be done via the pull request.

## Rebase

Rebasing is the tricky part of this process.

It's required when some changes happened on `master` after you created your branch.
One way to solve this would be to solve merge conflict but it might be complex and won't always work as expected if done via GitHub.

Instead, we propose here to rebase the incoming branch.

Before going any further, it is required to pull the last changes from `master`.
To do so:

```shell
git checkout master
git pull origin master
git checkout <your-branch>
```

Now you can proceed with `git rebase master -i` and follow the interactive documentation.

This process replays your branches commits from `master` so there's can't be conflict 
when merging the two branches and one keep a linear commit history.

Once the rebase is done, just push your branch to GitHub and have someone review your changes.
