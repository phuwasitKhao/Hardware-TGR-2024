import matplotlib.pyplot as plt

# Load data from the file
data = []
with open('spectrum_output.txt', 'r') as file:
    for line in file:
        index, value = line.split()
        data.append((int(index), float(value)))

# Separate the data into x and y
x, y = zip(*data)

# Plot the data
plt.plot(x, y, label="Spectrum")
plt.xlabel("Index")
plt.ylabel("Value")
plt.title("Spectrum Plot")
plt.legend()
plt.show()
