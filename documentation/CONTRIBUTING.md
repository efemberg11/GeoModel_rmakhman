# Contributing to the GeoModel Toolkit


## Issues

If you have an idea or feature that you want added or if you think there is an error **please [make an Issue][1] documenting it.**
Issues help a lot in both guiding what work gets done and documenting what improvements have been made.

If you have more time read on to see how to make a Merge Request to address your Issue.

[1]: https://gitlab.cern.ch/GeoModelDev/documentation/issues/new?issue


## Merge Requests

We are happy to accept contributions to the GeoModel Toolkit documentation via Merge Requests to the GitLab repo.
**To get started [fork this repo][2].**

[2]: https://gitlab.cern.ch/GeoModelDev/documentation/forks/new


### One Line Merge Requests

If you have a simple one line change you can use the [GitLab Web IDE][3] to make the change in your fork from your web browser.
The Web IDE button should be on the top right side of your fork's project page just above the SHA in the preview of the last commit.

Once you have made the change you want in the Web IDE, you can use the Web IDE "Commit" tab on the left side of the screen to stage your changes.
Once the changes are staged you can then click the "Commit..." button below them, enter your commit message, and then select the "Create a new branch and merge request" option.
From there, enter a descriptive branch name, such as

> revise/update-project-contact

and click the Commit button.
From there you will be prompted to open up a merge request.

[3]: https://docs.gitlab.com/ee/user/project/web_die/


### WIP

Unless you are making a single commit merge request please create a [WIP merge request][4] as soon as you start work and give an outline of the work in the description.
When you are close to being done please tag someone with merge permissions to follow the merge request and then ask for a review.

[4]: https://docs.gitlab.com/ee/user/project/merge_requests/work_in_progress_merge_requests.html


### Closing Related Issues

If there is an Issue that is related to the merge request please use the [GitLab Issue closing keywords][5] in the merge request Description to close that Issue when the merge request is accepted and merged.
As an example, if

> Closes #1

is in the merge request description then Issue #1 would automatically be closed by the merge request.

[5]: https://docs.gitlab.com/ee/user/project/issues/automatic_issue_closing.html


### Style

The website is [generated using MkDocs][6] and **requires** that [nested lists be indented by 4 spaces][7] in adherence with Markdown standards.
Please ensure that you follow this indenting style.

[6]: https://gitlab.cern.ch/authoring/documentation/mkdocs
[7]: https://commonmark.org/help/tutorial/10-nestedLists.html


### Testing

Before you submit your merge request for review you should test it locally first to make sure that MkDocs is rendering the Markdown properly.
To do this, use the [Docker image][8] provided by [CERN Authoring][9] that will be used by GitLab's CI system to build the website to test your changes locally.

This assumes you have Docker installed on your local machine.
If you don't, [install it][10] before continuing.

First pull down the latest image of the `stable` tag of the image

```
docker pull gitlab-registry.cern.ch/authoring/documentation/mkdocs:stable
```

Then from the top level directory of the repository run the following

```
. ./tests/test_build.sh
```

This will open the website built from the current state of the repository in your browser.
From here do a visual check to make sure that everything appears to be correct.
Please pay close attention to indenting of nested lists.

> Note: As the Docker image used runs as `root`, to remove the `_build` directory that is created you will either need `sudo` powers on your machine or you will need to run the image again to remove it:

```
docker run \
    --rm \
    -v $PWD:/build \
    -w /build \
    gitlab-registry.cern.ch/authoring/documentation/mkdocs:stable \
    /bin/ash -c "rm -rf _build"
```

[8]: https://gitlab.cern.ch/authoring/documentation/mkdocs/blob/master/Dockerfile
[9]: https://authoring.web.cern.ch/authoring/
[10]: https://docs.docker.com/install/
