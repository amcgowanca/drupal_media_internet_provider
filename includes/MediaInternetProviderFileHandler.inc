<?php
/**
 * @file
 * Contains the implementation of MediaInternetProviderFileHandler.inc
 */

/**
 * File handler for Media Internet Providers that allows complex URIs.
 */
class MediaInternetProviderFileHandler extends MediaInternetBaseHandler {
  /**
   * The file object representing the remote file.
   *
   * @var object
   */
  protected $file = NULL;

  /**
   * A boolean indicator used to determine if an error has occurred.
   *
   * @var bool
   */
  protected $error = FALSE;

  /**
   * {@inheritdoc}
   */
  public function preSave(&$file) {
    $path = parse_url($file->uri, PHP_URL_PATH);
    if (!$path) {
      $this->error = TRUE;
      throw new Exception(t('Unable to parse remote file uri: @remote', array(
        '@remote' => $file->uri,
      )));
    }

    $local_filename = basename($path);
    $local_filename = file_munge_filename($local_filename, media_internet_provider_file_allowed_extensions(), FALSE);
    $local_uri = file_stream_wrapper_uri_normalize('temporary://' . $local_filename);
    if (!@copy($file->uri, $local_uri)) {
      $this->error = TRUE;
      throw new Exception(t('Unable to copy remote file (@remote) to local destination: @local', array(
        '@remote' => $file->uri,
        '@local' => $local_uri,
      )));
    }
    $file = file_uri_to_object($local_uri);
  }

  /**
   * {@inheritdoc}
   */
  public function postSave(&$file) {
    if ($this->error) {
      throw new Exception(t('Unable to execute file move from temporary to destination as the handler has a previous error.'));
    }

    $scheme = file_default_scheme() . '://';
    module_load_include('inc', 'file_entity', 'file_entity.pages');
    $tmp_file = $file;
    $file_uri = file_stream_wrapper_uri_normalize(file_default_schema() . '://' . $file->filename);
    $file = file_move($file, $file_uri, FILE_EXISTS_RENAME);
    if (FALSE === $file) {
      throw new Exception(t('Unable to move temporary file (@tmp_file) to destination: @destination.', array(
        '@tmp_file' => $tmp_file->uri,
        '@destination' => $file_uri,
      )));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getFileObject() {
    if (!$this->file) {
      $this->file = file_uri_to_object($this->embedCode);
    }
    return $this->file;
  }

  /**
   * {@inheritdoc}
   */
  public function claim($url) {
    if ($scheme = parse_url($url, PHP_URL_SCHEME)) {
      $supported_schemes = variable_get('media_fromurl_supported_schemes', array('http', 'https', 'ftp', 'smb', 'ftps'));
      if (in_array($scheme, $supported_schemes)) {
        if ($path = parse_url($url, PHP_URL_PATH)) {
          $filename = basename($path);
          $regex = '/\.(' . preg_replace('/ +/', '|', preg_quote(media_internet_provider_file_allowed_extensions())) . ')$/i';
          if (preg_match($regex, $filename)) {
            return TRUE;
          }
        }
      }
    }
    return FALSE;
  }
}
