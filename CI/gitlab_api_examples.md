# GitLab API examples for GeoModel

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

Get only the pipelines for branch `master`:

```
curl  "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=master"
```

Get only the pipelines for branch `master` and with status `success`:

```
curl  "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=master&status=success"
```

Get the latest successful pipeline, just the latest one:

```
curl -v -H "Content-Type: application/json" "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=master&status=success&per_page=1&page=1"
```

To get the JSON content from the curl response, you can use `jq`, the JSON processor. On macOS, you can install `jq` with Homebrew: `brew install jq`. 

For example, get the `web_url` link for the latest successful pipeline:

```
curl -v -H "Content-Type: application/json" "https://gitlab.cern.ch/api/v4/projects/95156/pipelines?ref=master&status=success&per_page=1&page=1" | jq -r '.[0] | .web_url'
```

Then, to get the DAG page from the latest pipeline, just use the link from above and add “dag”:

https://gitlab.cern.ch/GeoModelDev/GeoModel/-/pipelines/2163714/dag

