## Drupal Media Internet Provider

Unlike the Drupal contributed module named Media Internet (submodule of [Media](https://www.drupal.org/project/media)) and its implementation of this handler, complex URIs containing query strings or fragments are not allowed nor is it capable of handling such URIs for remote files. This module aims to provide a basic implementation allowing for complex URIs for remote files containing query strings and fragments. Furthermore, due to the current implementation of Drupal core's implementation of determining file mimetypes, it is also required that the [Remote Stream Wrappers](https://www.drupal.org/project/remote_stream_wrapper) contributed module is used.

### License

This Drupal module is licensed under the [GNU General Public License](./LICENSE.md) version 2.
