# GitLab API examples for GeoModel

**NOTE:** Here below, `95156` is the ID of the GeoModel project on the CERN GitLab installation. You can find the ID of your project by looking at the main page of your GitLab repository, you'll find the ID under the name of the repository.

**NOTE:** Being the GeoModel respoitory public, we don't need authorization. If your project is not public, you need to pass a variable in the header. Please [see this post](https://stackoverflow.com/a/59949488/320369) for an example.


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

