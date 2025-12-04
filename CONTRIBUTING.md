# Contribute to openFrameworks

Thank you for contributing! openFrameworks is a community-built, maintained and operated project that welcomes contributions from all members of the openFrameworks community in its broadest sense. 

This document outlines the procedures and what to expect when contributing a bug report, a feature request, or new code via a pull request.
If you want to contribute documentation have a look at the [ofSite git repository](https://github.com/openframeworks/ofSite). 

**Please read** and **follow these guidelines** before submitting a bug report, feature request or pull request.
It really helps us efficiently process your contribution!

Jump to: [report a bug](#bug-reports), [request a feature](#feature-requests), [contribute code](#contributing-code).

## <a id='bug-reports'></a>Report a bug

We love hearing about bugs! It's how we get them fixed. 

- If you notice something odd happening, try to make it happen again (reproduce it). 
- If you can reproduce it, try to figure out if it's caused by your code or by openFrameworks. 
- If it's caused by openFrameworks, make sure it **still occurs in the current development version** (where we continually fix problems) by checking out with git or [downloading](https://github.com/openframeworks/openFrameworks/archive/master.zip) the current `master` branch of OF.
- If it looks like it's caused by openFrameworks (or if you're not sure) and still occurs on the current `master` branch, submit a bug report to the [issue tracker](https://github.com/openframeworks/openFrameworks/issues).
  - **Search the issue tracker** to make sure your problem has not been reported yet. If you find a relevant bug, comment there, even if it's an old or closed one! 
  - Only if you don't find a relevant issue, open a new issue. A new issue does not generate more exposure/visibility than commenting on an existing one.
  - Make sure you give it a good title! 
  A good title explains the core of the problem in about 5-10 words. (It's sometimes easier to write the title after you've written the description.)
    - Examples of bad titles: _problem with ofRectangle_ or _can't set color_
    - Examples of good titles: _ofRectangle crashes if the width is 0_ or _ofSetColor doesn't work with ofPath objects_.
  - In the description, include the following details:
    0. **relevant system information** such as which openFrameworks version, operating system, and IDE you are using,
    1. what you were doing when you noticed the bug,
    2. what you expected to happen,
    3. what actually happened, and
    4. how to make it happen again (ie how to reproduce the bug).
  - If you can demonstrate the bug in a few lines of code, include this code in the description. 
  But please __don't__ copy and paste your entire testApp file -- if you need to something like that, upload a zip file using a file locker service such as DropBox and provide a link.
- Someone will be along to review your bug report and assign tags to it.
You might be asked some questions about it, or you might not. 
If you don't hear back for a while don't despair, it's not being ignored!
We're simply a busy group of people, but you will hear back eventually.


## <a id='feature-requests'></a>Request a feature

At the moment we don't have a formal roadmap for the project, rather we are a community of people who each contributes to sections that we feel are important for the project.
Feature requests are therefore mostly a way of us discussing/feeling out together where we'd like the project to go. 
This can sometimes involve a lot of discussion, as everyone uses openFrameworks differently.

Feature requests are created as Github Issues, just like bugs. 
Feature requests are also where code that you or anyone else would like to include in a future pull request is **discussed before being implemented!**
If you're writing code to add a new feature that you think would be awesome to have in the core, that's great! 
But please make sure it's been discussed as a feature request _before_ you submit your pull request, as that increases the chances that your pull request will be accepted. 

Before opening a feature request, please search the issue tracker and confirm an existing request touching the same topic doesn't already exist.

- We are generally a friendly and open collection of people, but communication over the internet can be difficult, so please don't treat a lot of discussion as a negative point against your feature request. 
Usually, a lot of discussions just means that we haven't thought about what you're requesting before, which is a good thing!
- On the other hand, we are also busy people, often professionally involved in making large-scale projects ourselves, using openFrameworks. 
If you feel like no-one is paying attention to your feature request, just be patient, it will be considered eventually.
- And finally, a small reality check: please don't expect that a general agreement in the discussion that a feature request is a good idea means it will get made immediately! 
As with most open source projects, the fastest way to get a feature made is to make it yourself, or if you are not a programmer, make friends with someone who is, introduce them to openFrameworks, and ask them to make it for you.

## <a id='contributing-code'></a>Contribute code

We are more likely to accept your code if we feel like it has been discussed already. 
If you are submitting a new feature, it's best if the feature has been discussed beforehand, either as a [feature request](#feature-requests) or on the [forums](http://openframeworks.cc/forums) or the [of-dev mailing list](http://dev.openframeworks.cc/listinfo.cgi/of-dev-openframeworks.cc).

- Please read the [code style guidelines](https://github.com/openframeworks/openFrameworks/wiki/oF-code-style) and make sure your code conforms to them.
If in doubt, try and match the style and practices you find in the code you are working with.
- Please write _descriptive commit messages_ for each of the commits that you make.
They don't have to be in-depth, just a brief summary of what the commit contains. A page describing how well-written commit messages look like can be found [here](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

#### Organising your code


- Submit from a dedicated branch on your own repository **branched off from current `master`**. Your branch should be only about a single topic or area of openFrameworks. 
If you have multiple things to submit, make separate branches for each topic and submit multiple pull requests. 
(This makes it easier to review different parts of your code separately, and get it into the core faster.)
- The branch name should start with either __feature-__ for features or __bugfix-__ for bug fixes.
  - For example, if your patch adds code to draw ellipses, your branch should be called something like __feature-draw-ellipses__.
  - Remember, _commit early, commit often_ - use commits to isolate small subsets of code. 
This granularity makes the code easier to deal with in cases where some things have to be modified/isolated/removed from the pull request.
- When you commit your files and you find you can't do that without using `git add -f/--force`, this is because of the existing gitignore patterns. _Think about if those files really should be in the repo in the first place_. Then, instead of force-adding files which really should be in the repo (i.e. incorrectly match a gitignore pattern), correct the gitignore pattern (ask for help if necessary) and commit normally.
- Don't mix code style/formatting changes with bugfixes or features. They should be separate commits or better still, separate branches and separate pull requests.
- If you are able to do so, test your code on different platforms before submitting it, but at least test it on your platform to make sure it doesn't break anything.

#### Submitting the pull request

- If the default branch shown on the OF github repo is __`patch-release`__ submit your pull request to that branch if it is addressing a bug or regression. Otherwise submit your pull request to the __`master`__ branch of openFrameworks (which you branched off from), and _not_ the `stable` branch.
- All pull requests that contain changes that need to be in the changelog **must include relevant additions to `CHANGELOG.md`**. Use previous entries as a guide for style/indentation/etc.
- In the comments field on your new pull request, enter a description of everything that the code in the pull request does. 
  - This description is the first contact most of the core team will have with your code, so you should use it to explain why your pull request is awesome and we should accept it. 
  - Reference any issues or bugs in the [oF issue tracker](https://github.com/openframeworks/openFrameworks/issues) that are relevant to your pull request using `#issue number` notation, eg to reference issue __1234__ write `#1234`.
  - Mention if the code has been tested and on what platform.
- Your pull request will be evaluated via the [Pull Request Review Procedure](https://github.com/openframeworks/openFrameworks/wiki/Pull-Request-Review-Procedure)
