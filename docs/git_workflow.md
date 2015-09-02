# Git Workflow

## Introduction

This page documents the **version control workflow** followed by the openFrameworks community when dealing with the development of openFrameworks. 
**If you want to contribute**, and/or already have code or bug fixes you would like to see in the official repo, please **follow these guidelines** to make the life of the devs easier, minimize time needed for review, and ensure speedy and efficient incorporation of your improvements into oF.

## The workflow
The git workflow for openFrameworks is based on [this excellent article](http://nvie.com/posts/a-successful-git-branching-model/) detailing a very good branching model. 
**Read the article** first, it's quite complete and nicely presented, no need to replicate everything here.

A quick overview of the most important points (included oF-specific customizations):

* The `stable` branch contains the latest released, stable and tested version, and is only committed into on release of a new version.

* The `master` branch is where development/integration of external contributions happen. If you submit pull requests, branch off from and submit against `master`.

* `stable` and `master` are the only permanent branches in the repository.

* `feature-*` and `bugfix-*` branches are branched off from current `openFrameworks/master` by contributors. They only contain work on a certain feature or bug fix, respectively. They **don't** live in the openframeworks repository. Completed work is merged back into `openFrameworks/master` from the newly created branch with a pull request.

* Integrators, when possible, merge with the `--no-ff` option (or using the Merge button in the github web interface) to create a new commit even in fast-forward merges, to improve the information in the git history about the newly merged branch. **After merging, confirm that any connected and solved issues are closed in the issue tracker.**

* `hotfix-*` branches are created when a critical bug in a released version must be resolved immediately. They are branched off from `stable` instead of `master` when necessary to avoid polluting the release fix with any features/fixes for the next milestone, which potentially have already been merged into `master`.

* `release-*` branches support preparation of a new production release if needed. They allow for last-minute dotting of i’s and crossing t’s, and minor bug-fixing.

## How to contribute
If you want to contribute to OF, you typically do the following:

 * Normally, changes/bugs/features are discussed in an issue in the issue tracker before submitting a pull request (PR), so if there's no issue open, make one, putting your changes to the OF community for discussion.
Check that nobody else is working on the same issue (i.e. no open PRs) to minimize duplicated work.

 * Make sure that the `master` branch in your fork is in sync with `openFrameworks/master`. **Branch off from current `master`**, choosing a fitting name for the new branch (e.g. `feature-newEventModel`).

 * Work on your contribution in the new branch. Remember one rule of git: **Commit early, commit often!** Otherwise, it is very difficult for others to review what you have done. All logically separate chunks in your work should go into a separate commit, that way each snippet can be checked more easily. 

 * While working, [write well-formatted commit messages](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

 * If your work fixes a certain bug which is in the issue tracker, be sure to include `fixes #<issueNr>` or `closes #<issueNr>` in one of the (probably the final) _commit messages_. Then [said issue will be auto-closed](https://github.com/blog/831-issues-2-0-the-next-generation) as soon as the commit is merged. Sadly, including this in the PR description alone does _NOT_ work.

 * Make sure your code conforms to the [[OF code style guideline|openFrameworks Coding style guidelines]]. Non-syntax-changing, whitespace-only changes ( code style) should always be in a separate commit, otherwise it's more difficult to isolate who has changed what in the code history. Choose a commit message like "Format following code style". If you only touched a small portion of a file, try not to change all the file by just running the code style script. (This will not be much of a problem soon, when we have made all of OF conform to the code style)

 * When you commit your files and you find you can't do that without using `git add -f/--force`, this is because of the existing gitignore patterns. 
**Think about if those files really should be in the repo** in the first place. Then, instead of force-adding files which really should be in the repo (i.e. incorrectly match a gitignore pattern), correct the gitignore pattern and commit normally, or at least ping the [github section leader](http://www.openframeworks.cc/about) about the necessary change so that he/she can take care of it.
The benefits of keeping the gitignore structure in sync with files in the repo is that falsely committed files and/or files which are to be falsely committed are more easily spotted.

 * **Test your changes** to make sure they work and fix the bug.

 * When you're satisfied with your work, and there were commits on `openFrameworks/master` in the meantime, merge `openframeworks/master` into _your_ branch to ensure that your new work will apply cleanly onto `openFrameworks/master`. 

 * If your changes are such that they need to be reported in the release changelog, you must add the relevant information to `CHANGELOG.md`. This will help us avoid the changelog preparation bottleneck that always happens before a release. Use previous entry as a guide for style/indentation/etc.

 * Then, from the new branch, send a **pull request against `openFrameworks/master`**. Describe the bug (e.g. by referring to an existing issue using the GH `#<issuenr>` syntax), and indicate if you have tested the changes, and any other relevant details.

 * Collaborate with the devs on successfully integrating your work. If more work is needed, or there are open question about our changes, this will be discussed in the comment section of the pull request. If you need to add more commits to your PR, just commit to your PR branch and push it to your repo, and the PR will automatically get amended/updated.

## General git resources:

### Format of commit messages:
[This page](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html) contains some explanations about a good format for commit messages.

### Git docs
**If you have just discovered git**, or want to learn some tricks, there are a couple of resources you should look at (roughly in the order you should read them):

[The github trial site](http://try.github.com/): Gives you a nice tour of git. Get your feet wet without even installing software!

[The github help page](http://help.github.com/): Deals with basis usage, concepts and terms of git and github. Good to get a first idea.

[gitref.org](http://gitref.org/): Nice and concise reference of the essential functions of git. Takes about 30min to read through, you'll come out smarter at the end.

[git-scm.com/](http://git-scm.com/): A new central homepage for all things git. Contains git books, reference, downloads, information on GUIs, etc.

[The git community book on git-scm.com](http://git-scm.com/book/): This book is meant to help you learn how to use Git as quickly and easily as possible. The former git community book and the ProGit book have been merged into this one.

[The oF git cheat sheet](http://piratepad.net/ofgitcheatsheet): A small list of helpful git commands put together by oF developers.

[An interactive cheat sheet](http://ndpsoftware.com/git-cheatsheet.html#loc=index;): Shows/explains the most common commands for using git.

[A git man pages mirror](http://schacon.github.com/git/git.html): The manual of git. Also available on your command line, but much more readable on the net. :D Bare-bones explanation of the details of command line git usage.

If you ever have a question about how to do things in git, it is often best to enter your question on [Stack overflow](http://www.stackoverflow.com). Most probably someone there has had your problem before. For example, [How do I make git ignore mode changes?](http://stackoverflow.com/search?q=How+do+I+make+git+ignore+mode+changes)

There is also a great amount of articles, wiki pages, tutorials and blog posts about git, for example [git for the lazy](http://spheredev.org/wiki/Git_for_the_lazy) or [Git Immersion](http://library.edgecase.com/git_immersion/index.html).

### Tips and Tricks
* When comparing/analysing code, it's sometimes desirable to ignore whitespace/code style changes. You can do this when using git by using the `-w` option when doing `git diff` or most other relevant git commands. On the Github web interface, all you have to do is append `?w=1` to any relevant URL, and whitespace changes are ignored.

* When you want to make tests on a submitted PR, it's often necessary to pull it into your repo. A quick way to do this is by fetching the PR's branch without adding a remote you'll probably only need once:
`git checkout master`, `git fetch git://github.com/<username>/openFrameworks.git <branchname>`. Then merge into master: `git merge FETCH_HEAD`. `FETCH_HEAD` is a ref which is automatically created when fetching. Now you can test the version just like it would get merged if you hit the "Merge" button.

* Another way to quickly check out *pull requests* is `git fetch upstream pull/7324/head:pr-7324`, where `upstream` is the OF repo remote, `pull/7324/head` is the remote pull request, and `pr-7324` is the local pull-request branch.

* Show files which are in the repo, but should actually be ignored, i.e. they match a gitignore pattern and have probably been force-added: `git ls-files -i --exclude-standard`. Great for tweaking/cleaning up the gitignore rules.

This doc copied from https://github.com/openframeworks/openFrameworks/wiki/openFrameworks-git-workflow