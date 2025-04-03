# process_wav.py

import os
import argparse
import wave
import struct
import math
from collections import defaultdict, Counter

prob_file_name = "ngram_prob_python.txt" # file name to store the probabilities

def brainwire_quant(v):
    return int(math.floor(v / 64.0))

def brainwire_dequant(v):
    if v >= 0:
        return round(v * 64.061577 + 31.034184)
    else:
        return -(round((-v - 1) * 64.061577 + 31.034184) - 1)

def read_wav_file(path):
    with wave.open(path, 'rb') as wav:
        n_channels = wav.getnchannels()
        sampwidth = wav.getsampwidth()
        n_frames = wav.getnframes()
        sample_rate = wav.getframerate()

        if sampwidth != 2:
            raise ValueError("Only 16-bit WAV files are supported.")

        raw_data = wav.readframes(n_frames)
        samples = struct.unpack('<' + 'h' * (len(raw_data) // 2), raw_data)
        return samples

def process_file(path, ngram_size, stats):
    samples = read_wav_file(path)
    quantized = [brainwire_quant(s) for s in samples]

    for i in range(len(quantized) - ngram_size + 1):
        ngram = tuple(quantized[i:i+ngram_size])
        stats[ngram] += 1

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', '--ngram', type=int, default=1, help="n-gram size")
    args = parser.parse_args()

    directory = './data'
    stats = defaultdict(int)

    print("Reading files...")
    for filename in os.listdir(directory):
        if filename.endswith('.wav'):
            filepath = os.path.join(directory, filename)
            process_file(filepath, args.ngram, stats)

    total_count = sum(stats.values())
    sorted_probs = sorted(((k, v / total_count) for k, v in stats.items()), key=lambda x: -x[1])

    with open(prob_file_name, "w") as f:
        f.write("Symbol Probabilities:\n")
        for symbol, prob in sorted_probs:
            f.write(f"Symbol: {list(symbol)} | Probability: {prob:.20f}\n")

    print(f"\nSum of all probabilities: {sum(prob for _, prob in sorted_probs)}")
    print(f"Number of unique symbols: {len(sorted_probs)}")

if __name__ == "__main__":
    main()
