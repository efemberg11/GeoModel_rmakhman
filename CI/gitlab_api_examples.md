# GitLab API examples for GeoModel

**NOTE:** Here below, `95156` is the ID of the GeoModel project on the CERN GitLab installation. You can find the ID of your project by looking at the main page of your GitLab repository, you'll find the ID under the name of the repository.

**NOTE:** Being the GeoModel respoitory public, we don't need authorization. If your project is not public, you need to pass a variable in the header. Please [see this post](https://stackoverflow.com/a/59949488/320369) for an example.
But please notice, to get CI **jobs** through the API, you need at least a Personal Access Token, even if the repository is fully public. 

[TOC]

## Tags 

### Get the list of tags 

```
curl "https://gitlab.cern.ch/api/v4/projects/95156/repository/tags/"
```

### Get the name of the latest tag 

```
curl -Ss --request GET "https://gitlab.cern.ch/api/v4/projects/95156/repository/tags" | jq -r '.[0] | .name'
```

### Get the commit hash of the latest tag 

```
curl -Ss --request GET "https://gitlab.cern.ch/api/v4/projects/95156/repository/tags" | jq -r '.[0] | .commit.id'
```

## Pipelines

Given the GeoModel project ID: 95156, get all the latest pipelines:

```
curl  "https://gitlab.cern.ch/api/v4/projects/95156/pipelines"
```

The answer is:

```
[{"id":2164874,"sha":"860bdc5c0298e6d7d8761adaef1fb10140530743","ref":"master-ci-add-mac-jobs","status":"success","created_at":"2020-12-09T17:19:50.681+01:00","updated_at":"2020-12-09T18:01:19.091+01:00","web_url":"https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/2164874"},
{"id":2164665,"sha":"11e71620dae9e3dd8ff379b821afe1061dab8d60","ref":"master-ci-add-mac-jobs","status":"success","created_at":"2020-12-09T16:27:26.643+01:00","updated_at":"2020-12-09T17:23:34.636+01:00","web_url":"https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/2164665"},
...
```

Get only the pipelines for branch `main`:

```
curl  "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=main"
```

Get only the pipelines for branch `main` and with status `success`:

```
curl  "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=main&status=success"
```

Get the latest successful pipeline, just the latest one:

```
curl -v -H "Content-Type: application/json" "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=main&status=success&per_page=1&page=1"
```

To get the JSON content from the curl response, you can use `jq`, the JSON processor. On macOS, you can install `jq` with Homebrew: `brew install jq`. 

For example, get the `web_url` link for the latest successful pipeline:

```
curl -v -H "Content-Type: application/json" "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=main&status=success&per_page=1&page=1" | jq -r '.[0] | .web_url'
```

Then, to get the DAG page from the latest pipeline, just use the link from above and add “dag”:

https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/2163714/dag


## Get a CI job or list of jobs


### Get a single job and its metadata

We can ask for the latest CI job with:

```sh
curl -Ss --header "PRIVATE-TOKEN:<token>" "https://gitlab.cern.ch/api/v4/projects/95156/jobs/?page=1&per_page=1"
```

this gives:

```
[{"id":32216368,"status":"success","stage":"step-E","name":"ubu-single-gmfsl","ref":"main","tag":false,"coverage":null,"allow_failure":false,"created_at":"2023-09-04T18:38:51.723+02:00","started_at":"2023-09-04T19:10:52.976+02:00","finished_at":"2023-09-04T19:14:59.394+02:00","erased_at":null,"duration":246.417715,"queued_duration":0.475744,"user":{"id":281,"username":"rbianchi","name":"Riccardo Maria Bianchi","state":"active","avatar_url":"https://gitlab.cern.ch/uploads/-/system/user/avatar/281/avatar.png","web_url":"https://gitlab.cern.ch/rbianchi","created_at":"2015-04-30T11:00:28.847+02:00","bio":"Particle Physicist (PhD) at CERN working on the ATLAS experiment (since 2005). Faculty Research Associate at the University of Pittsburgh. Co-author of \"Applied Computational Physics\" (Oxford University Press, 2018).","location":"CERN, Geneva, Switzerland","public_email":"riccardo.maria.bianchi@cern.ch","skype":"","linkedin":"riccardo-maria-bianchi-8580432a","twitter":"drric_physics","discord":"","website_url":"www.riccardomariabianchi.com","organization":"ATLAS Experiment, University of Pittsburgh","job_title":"","pronouns":"","bot":false,"work_information":"ATLAS Experiment, University of Pittsburgh","followers":0,"following":0,"local_time":"8:17 PM"},"commit":{"id":"fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a","short_id":"fdafc154","created_at":"2023-09-04T18:38:49.000+02:00","parent_ids":["ffe3884fb7e1a1903fcc17400430137cb12b311e"],"title":"CI: add expiry policy of 1 day to all artifacts","message":"CI: add expiry policy of 1 day to all artifacts\r\n\r\nThis will help the fix of #88","author_name":"Riccardo Maria Bianchi","author_email":"riccardo.maria.bianchi@cern.ch","authored_date":"2023-09-04T18:38:49.000+02:00","committer_name":"Riccardo Maria Bianchi","committer_email":"riccardo.maria.bianchi@cern.ch","committed_date":"2023-09-04T18:38:49.000+02:00","trailers":{},"web_url":"https://gitlab.cern.ch/GeoModelDev/GeoModel/-/commit/fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a"},"pipeline":{"id":6150944,"iid":1561,"project_id":95156,"sha":"fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a","ref":"main","status":"success","source":"push","created_at":"2023-09-04T18:38:51.434+02:00","updated_at":"2023-09-04T19:57:43.438+02:00","web_url":"https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/6150944"},"web_url":"https://gitlab.cern.ch/GeoModelDev/GeoModel/-/jobs/32216368","project":{"ci_job_token_scope_enabled":false},"artifacts_file":{"filename":"artifacts.zip","size":1381606499},"artifacts":[{"file_type":"archive","size":1381606499,"filename":"artifacts.zip","file_format":"zip"},{"file_type":"metadata","size":995592,"filename":"metadata.gz","file_format":"gzip"},{"file_type":"trace","size":145865,"filename":"job.log","file_format":null}],"runner":{"id":33042,"description":"default-runner-69f5dcf6c6-hsn54","ip_address":"188.185.15.101","active":true,"paused":false,"is_shared":true,"runner_type":"instance_type","name":"gitlab-runner","online":true,"status":"online"},"artifacts_expire_at":"2023-09-05T19:14:48.655+02:00","tag_list":["docker"]}]
```

But we can prettify the output with `jq` itself (note teh dot at the end!):

```sh
curl -Ss --header "PRIVATE-TOKEN:<token>" "https://gitlab.cern.ch/api/v4/projects/95156/jobs/?page=1&per_page=1" | jq .
```

which gives this nice output:

```json
[
  {
    "id": 32216368,
    "status": "success",
    "stage": "step-E",
    "name": "ubu-single-gmfsl",
    "ref": "main",
    "tag": false,
    "coverage": null,
    "allow_failure": false,
    "created_at": "2023-09-04T18:38:51.723+02:00",
    "started_at": "2023-09-04T19:10:52.976+02:00",
    "finished_at": "2023-09-04T19:14:59.394+02:00",
    "erased_at": null,
    "duration": 246.417715,
    "queued_duration": 0.475744,
    "user": {
      "id": 281,
      "username": "rbianchi",
      "name": "Riccardo Maria Bianchi",
      "state": "active",
      "avatar_url": "https://gitlab.cern.ch/uploads/-/system/user/avatar/281/avatar.png",
      "web_url": "https://gitlab.cern.ch/rbianchi",
      "created_at": "2015-04-30T11:00:28.847+02:00",
      "bio": "Particle Physicist (PhD) at CERN working on the ATLAS experiment (since 2005). Faculty Research Associate at the University of Pittsburgh. Co-author of \"Applied Computational Physics\" (Oxford University Press, 2018).",
      "location": "CERN, Geneva, Switzerland",
      "public_email": "riccardo.maria.bianchi@cern.ch",
      "skype": "",
      "linkedin": "riccardo-maria-bianchi-8580432a",
      "twitter": "drric_physics",
      "discord": "",
      "website_url": "www.riccardomariabianchi.com",
      "organization": "ATLAS Experiment, University of Pittsburgh",
      "job_title": "",
      "pronouns": "",
      "bot": false,
      "work_information": "ATLAS Experiment, University of Pittsburgh",
      "followers": 0,
      "following": 0,
      "local_time": "8:18 PM"
    },
    "commit": {
      "id": "fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a",
      "short_id": "fdafc154",
      "created_at": "2023-09-04T18:38:49.000+02:00",
      "parent_ids": [
        "ffe3884fb7e1a1903fcc17400430137cb12b311e"
      ],
      "title": "CI: add expiry policy of 1 day to all artifacts",
      "message": "CI: add expiry policy of 1 day to all artifacts\r\n\r\nThis will help the fix of #88",
      "author_name": "Riccardo Maria Bianchi",
      "author_email": "riccardo.maria.bianchi@cern.ch",
      "authored_date": "2023-09-04T18:38:49.000+02:00",
      "committer_name": "Riccardo Maria Bianchi",
      "committer_email": "riccardo.maria.bianchi@cern.ch",
      "committed_date": "2023-09-04T18:38:49.000+02:00",
      "trailers": {},
      "web_url": "https://gitlab.cern.ch/GeoModelDev/GeoModel/-/commit/fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a"
    },
    "pipeline": {
      "id": 6150944,
      "iid": 1561,
      "project_id": 95156,
      "sha": "fdafc154d8aa4a8f9f6eefe99d906f623ad99f8a",
      "ref": "main",
      "status": "success",
      "source": "push",
      "created_at": "2023-09-04T18:38:51.434+02:00",
      "updated_at": "2023-09-04T19:57:43.438+02:00",
      "web_url": "https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/6150944"
    },
    "web_url": "https://gitlab.cern.ch/GeoModelDev/GeoModel/-/jobs/32216368",
    "project": {
      "ci_job_token_scope_enabled": false
    },
    "artifacts_file": {
      "filename": "artifacts.zip",
      "size": 1381606499
    },
    "artifacts": [
      {
        "file_type": "archive",
        "size": 1381606499,
        "filename": "artifacts.zip",
        "file_format": "zip"
      },
      {
        "file_type": "metadata",
        "size": 995592,
        "filename": "metadata.gz",
        "file_format": "gzip"
      },
      {
        "file_type": "trace",
        "size": 145865,
        "filename": "job.log",
        "file_format": null
      }
    ],
    "runner": {
      "id": 33042,
      "description": "default-runner-69f5dcf6c6-hsn54",
      "ip_address": "188.185.15.101",
      "active": true,
      "paused": false,
      "is_shared": true,
      "runner_type": "instance_type",
      "name": "gitlab-runner",
      "online": true,
      "status": "online"
    },
    "artifacts_expire_at": "2023-09-05T19:14:48.655+02:00",
    "tag_list": [
      "docker"
    ]
  }
]
```

From the output above it is easy to spot useful metadata to exploit in API calls and scripts!

