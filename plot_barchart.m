%% --- Plot the barchats to visualise the probability distribution:

filename = './single_symbol_probabilities.txt';
data = readtable(filename, 'Delimiter', '|', 'ReadVariableNames', false);

symbols = [];
probabilities = [];
fileID = fopen(filename, 'r');
while ~feof(fileID)
    line = fgetl(fileID);
    if contains(line, 'Symbol:') && contains(line, 'Probability:')
        tokens = regexp(line, 'Symbol:\s*(-?\d+)\s*\|.*Probability:\s*([\d\.eE+-]+)', 'tokens');
        if ~isempty(tokens)
            symbols(end+1) = str2double(tokens{1}{1});
            probabilities(end+1) = str2double(tokens{1}{2});
        end
    end
end
fclose(fileID);

[sorted_probs, idx_prob] = sort(probabilities, 'descend');
sorted_symbols_prob = symbols(idx_prob);

[sorted_symbols_sym, idx_sym] = sort(symbols, 'ascend');
sorted_probs_sym = probabilities(idx_sym);

figure;
bar(1:length(sorted_symbols_prob), sorted_probs);
xlabel('Index (Sorted by Probability)');
ylabel('Probability');
title('Symbols vs Probabilities (Sorted by Probability)');
grid minor;
hold on;
yline(1/1024, 'r--', 'y = 1/1024');
hold off;

figure;
bar(sorted_symbols_sym, sorted_probs_sym);
xlabel('Symbols');
ylabel('Probability');
title('Symbols vs Probabilities (Sorted by Symbol)');
grid minor;
hold on;
yline(1/1024, 'r--', 'y = 1/1024');
hold off;
