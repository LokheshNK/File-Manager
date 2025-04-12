import matplotlib.pyplot as plt
from collections import defaultdict
import os

log_file = "file_usage_log.txt"

# Ensure the file exists
if not os.path.exists(log_file):
    print("No file usage log found.")
    exit()

usage_data = defaultdict(int)

with open(log_file, "r") as file:
    for line in file:
        parts = line.strip().split(",")

        # If the line contains a duration, extract it
        if len(parts) >= 5 and parts[3] == "duration":
            file_name = parts[0].split("/")[-1]  # Extract file name
            try:
                duration = int(parts[4].split()[0])  # Extract duration in seconds
                usage_data[file_name] += duration
            except ValueError:
                print(f"Skipping invalid line: {line.strip()}")

        # If the line only contains "opened" status, assume the file is still open
        elif len(parts) == 3 and parts[1] == "opened":
            file_name = parts[0].split("/")[-1]  # Extract file name
            usage_data[file_name] += 60  # Assume a minimum usage time (e.g., 1 min)

# Check if any data is available
if not usage_data:
    print("No data available.")
    exit()

# Plot data
plt.figure(figsize=(8, 6))
plt.pie(usage_data.values(), labels=usage_data.keys(), autopct="%1.1f%%")
plt.title("Digital Well-Being: File Usage Duration")
plt.show()
