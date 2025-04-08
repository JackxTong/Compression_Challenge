from process_wav import read_wav_file, brainwire_quant

def load_probabilities(filepath):
    probs = {}
    with open(filepath, 'r') as f:
        for line in f:
            if line.startswith("Symbol:"):
                symbol_str, prob_str = line.strip().split('|')
                symbol = tuple(map(int, symbol_str.split('[')[1].split(']')[0].split(',')))
                prob = float(prob_str.split(':')[-1])
                probs[symbol] = prob
    return probs

def build_cdf(probs):
    sorted_items = sorted(probs.items())
    cdf = {}
    total = 0.0
    for symbol, prob in sorted_items:
        cdf[symbol] = (total, total + prob)
        total += prob
    return cdf

def arithmetic_encode(symbols, cdf):
    low = 0.0
    high = 1.0

    for symbol in symbols:
        if symbol not in cdf:
            raise ValueError(f"Unknown symbol: {symbol}")
        range_width = high - low
        sym_low, sym_high = cdf[symbol]
        high = low + range_width * sym_high
        low = low + range_width * sym_low

    # final code (a float between 0 and 1)
    return (low + high) / 2  

def arithmetic_decode(code, cdf, total_symbols):
    symbols = []
    for _ in range(total_symbols):
        for symbol, (sym_low, sym_high) in cdf.items():
            if sym_low <= code < sym_high:
                symbols.append(symbol)
                range_width = sym_high - sym_low
                code = (code - sym_low) / range_width
                break
        else:
            raise ValueError("Decoding failed: code not in any interval")
    return symbols

