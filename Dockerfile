# Use a lightweight Windows or Linux base image
FROM mcr.microsoft.com/windows/nanoserver:ltsc2019

# Set working directory
WORKDIR /app

# Copy the executable into the container
COPY startship.exe .

# Set the default command to run the game
CMD ["startship.exe"]
