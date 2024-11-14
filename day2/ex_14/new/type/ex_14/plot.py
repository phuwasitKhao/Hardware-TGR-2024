import matplotlib.pyplot as plt

def read_frequency_data(filename="frequency_output.txt"):
    """
    Reads frequency data from a text file.

    Parameters:
        filename (str): The name of the file to read the data from.

    Returns:
        list: List of amplitude values.
    """
    values = []
    with open(filename, "r") as file:
        for line in file:
            _, value = line.split()
            values.append(float(value))
    return values

def plot_frequency_spectrum(amplitudes, sampling_rate, fft_length):
    """
    Plots the frequency spectrum with x-axis as Hz and y-axis as amplitude.

    Parameters:
        amplitudes (list): List of amplitude values from the FFT.
        sampling_rate (int): The sampling rate of the original signal.
        fft_length (int): The length of the FFT (number of points).
    """
    # Compute frequency values for the x-axis (only for the first half of the FFT result)
    frequencies = [(i * sampling_rate / fft_length) for i in range(len(amplitudes))]
    
    # Plot the frequency spectrum
    plt.plot(frequencies, amplitudes, label="Frequency Spectrum")
    plt.xlabel("Frequency (Hz)")
    plt.ylabel("Amplitude")
    plt.title("Frequency Spectrum Plot")
    plt.legend()
    plt.show()

# Example usage
sampling_rate = 44100  # Replace with your actual sampling rate in Hz
fft_length = 4096      # Replace with your actual FFT length
amplitudes = read_frequency_data("frequency_output.txt")
plot_frequency_spectrum(amplitudes, sampling_rate, fft_length)
