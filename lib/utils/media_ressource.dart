import 'package:flutter_webrtc/flutter_webrtc.dart';
import 'package:uuid/uuid.dart';

class MediaRessource {
  static MediaStream? _globalVideoStream;
  static MediaStream? _globalAudioStream;
  static int _audioListenerCount = 0;
  static int _videoListenerCount = 0;

  MediaStream? _stream;

  MediaStream? get stream {
    return _stream;
  }

  Future<MediaStream?> open(bool audio, bool video) async {
    if (null != _stream) {
      return _stream;
    }

    if (audio) {
      _globalAudioStream ??= await navigator.mediaDevices.getUserMedia({
        'audio': true,
        'video': false,
      });
      _audioListenerCount++;
    }
    if (video) {
      _globalVideoStream ??= await navigator.mediaDevices.getUserMedia({
        'audio': false,
        'video': true,
      });
      _videoListenerCount++;
    }

    _stream = await createLocalMediaStream(const Uuid().v4());

    if (audio) {
      _globalAudioStream?.getAudioTracks().forEach((track) {
        _stream?.addTrack(track);
      });
    }

    if (video) {
      _globalVideoStream?.getVideoTracks().forEach((track) {
        _stream?.addTrack(track);
      });
    }

    return _stream;
  }

  void close() {
    if (null == _stream) {
      return;
    }

    if (_stream?.getAudioTracks().isNotEmpty ?? false) {
      _audioListenerCount--;
    }

    if (_stream?.getVideoTracks().isNotEmpty ?? false) {
      _videoListenerCount--;
    }

    _stream = null;

    if (_audioListenerCount <= 0) {
      _globalAudioStream?.getTracks().forEach((track) {
        track.stop();
      });
      _globalAudioStream?.dispose();
      _globalAudioStream = null;
    }

    if (_videoListenerCount <= 0) {
      _globalVideoStream?.getTracks().forEach((track) {
        track.stop();
      });
      _globalVideoStream?.dispose();
      _globalVideoStream = null;
    }
  }
}
