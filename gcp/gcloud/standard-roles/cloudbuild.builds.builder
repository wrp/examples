description: Can perform builds
etag: AA==
includedPermissions:
- artifactregistry.files.get
- artifactregistry.files.list
- artifactregistry.packages.get
- artifactregistry.packages.list
- artifactregistry.repositories.downloadArtifacts
- artifactregistry.repositories.get
- artifactregistry.repositories.list
- artifactregistry.repositories.uploadArtifacts
- artifactregistry.tags.create
- artifactregistry.tags.get
- artifactregistry.tags.list
- artifactregistry.tags.update
- artifactregistry.versions.get
- artifactregistry.versions.list
- cloudbuild.builds.create
- cloudbuild.builds.get
- cloudbuild.builds.list
- cloudbuild.builds.update
- logging.logEntries.create
- pubsub.topics.create
- pubsub.topics.publish
- remotebuildexecution.blobs.get
- resourcemanager.projects.get
- resourcemanager.projects.list
- source.repos.get
- source.repos.list
- storage.buckets.create
- storage.buckets.get
- storage.buckets.list
- storage.objects.create
- storage.objects.delete
- storage.objects.get
- storage.objects.list
- storage.objects.update
name: roles/cloudbuild.builds.builder
stage: GA
title: Cloud Build Service Account