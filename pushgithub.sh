#!/bin/sh

setup_git() {
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis CI"
  git config remote.origin.url https://$GH_TOKEN@github.com/sudtanj/MPICH-Ubuntu.git
}

commit_website_files() {
  git add . *
  git commit --message "Travis build: $TRAVIS_BUILD_NUMBER"
}

upload_files() {
   git push origin HEAD:master
}

setup_git
commit_website_files
upload_files