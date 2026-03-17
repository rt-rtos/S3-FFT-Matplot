# serial_binary.py

Read framed FFT data from an ESP32-S3 over USB-CDC or UART, validate each frame, and turn the stream into CSV rows, live plots, optional audio, and debug output.

## What It Does

This script is a host-side reader for the binary frame format produced by the firmware. It:

- opens a serial port and reads the incoming byte stream in a background thread
- searches for the frame magic bytes and checks the CRC32 before accepting a frame
- extracts frame metadata such as timestamp, sample rate, FFT size, and channel label
- keeps the raw float32 payload intact so it can be plotted, logged, or sonified without extra copies
- optionally reconnects after the device resets
- optionally prints frame metadata and payload hex for debugging

The payload format depends on the frame version:

- version 1: magnitude-only spectra with `fft / 2` float32 values
- version 2: complex spectra with `fft` float32 values packed as `DC, Nyquist, Re1, Im1, Re2, Im2, ...`

## Quick Start

From a fresh clone:

```bash
git clone ...
cd repo
chmod +x serial_binary.py    # one time
./serial_binary.py
```

If the script lives under `tools/`, run the same commands from that directory or invoke it as `python tools/serial_binary.py`.

On Windows, the easiest equivalent is usually:

```pwsh
uv run tools/serial_binary.py
```

## Requirements

- Python 3.14 or newer
- `uv` if you want to run the script directly from its shebang
- `pyserial` for serial access
- `numpy`, `matplotlib`, and `sounddevice` if you use plotting or audio output

The script declares its own runtime dependencies in the file header, so `uv` can install them on demand.

## Command-Line Options

Run `./serial_binary.py --help` to see the parser help.

| Flag | Default | Description |
| --- | --- | --- |
| `port` | `COM8` | Serial port to open, such as `COM8` or `/dev/ttyACM0`. |
| `--baud` | `921600` | Baud rate used for UART connections. USB-CDC ignores this value. |
| `--output` | `stream.csv` | Output file used when CSV logging is enabled. |
| `--append` | off | Append to the output file instead of truncating it. |
| `--max-frames` | `0` | Stop after N frames. `0` means run until interrupted. |
| `--flush-every` | `50` | Flush the output file every N frames. |
| `--plot` | on | Enable live plotting. |
| `--plot-interval` | `2` | Plot refresh interval in milliseconds. |
| `--max-freq` | `22000` | Upper frequency limit shown in plots and used by audio synthesis. `0` disables the cutoff. |
| `--separate-plots` | off | Use separate windows for the time and frequency plots. |
| `--audio` | off | Play an audio representation of the selected FFT channel. |
| `--audio-rate` | `44100` | Output sample rate for audio synthesis. |
| `--audio-block` | `2048` | Audio block size in samples. |
| `--audio-gain` | `0.2` | Gain applied to the synthesized audio. |
| `--audio-max` | `0.3` | Limiter ceiling for synthesized audio. `0` disables limiting. |
| `--audio-normalize` | on | Normalize each audio block to `--audio-max` when possible. |
| `--audio-channel` | `ch0` | Channel to send to the audio path. Choose `ch0` or `ch1`. |
| `--no-csv` | on | Disable CSV output. |
| `--reconnect` | on | Reconnect automatically if the device disconnects or resets. |
| `--reconnect-delay` | `0.5` | Seconds to wait between reconnect attempts. |
| `--no-plot-freq` | off | Disable the frequency-domain plot. |
| `--no-plot-time` | off | Disable the time-domain plot. |
| `-w`, `--window` | `0` | Window function for host-side processing. `0=None`, `1=Hann`, `2=Hamming`, `3=Blackman`. |
| `--inspect-frames` | off | Print frame metadata and sample values for debugging. |
| `--hex-dump` | off | Show a truncated hex dump of the payload while inspecting frames. |
| `--read-chunk` | `16384` | Serial read chunk size in bytes. |
| `--read-max` | `262144` | Maximum bytes to read per loop iteration. |
| `--stats-interval` | `5.0` | Seconds between host-side throughput reports. `0` disables stats. |

## Typical Usage

Read from the default port and show the live plot:

```bash
./serial_binary.py
```

Capture a fixed number of frames and stop:

```bash
./serial_binary.py COM8 --max-frames 100
```

Inspect the frame structure and payload contents:

```bash
./serial_binary.py COM8 --inspect-frames --hex-dump --max-frames 5
```

Play audio from channel 1 instead of plotting:

```bash
./serial_binary.py COM8 --audio --audio-channel ch1
```

## Notes

- CSV logging is disabled by default in the current script because `--no-csv` is enabled unless the parser is changed.
- Live plotting is enabled by default.
- Automatic reconnect is enabled by default so the reader can survive USB resets.
- The script is optimized for the ESP32-S3 binary frame stream and expects the firmware to keep the frame layout stable.
