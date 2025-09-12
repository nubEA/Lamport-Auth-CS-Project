# docker run -it --rm --net=host \
#   -e WAYLAND_DISPLAY=$WAYLAND_DISPLAY \
#   -e XDG_RUNTIME_DIR=$XDG_RUNTIME_DIR \
#   -v $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY:$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY \
#   lamport-image

# --- STAGE 1: The "Builder" ---
# This stage has all the tools needed to compile the application
FROM fedora:latest AS builder

# Install all build-time dependencies
RUN dnf install -y \
    gcc-c++ \
    cmake \
    make \
    qt5-qtbase-devel \
    cryptopp-devel \
    qt5-qtwayland

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . .

# Perform an out-of-source build
RUN mkdir build && cd build && cmake .. && make


# --- STAGE 2: The Final Image ---
# This is the small, clean image we will actually use
FROM fedora:latest

# Install ONLY the runtime dependencies. Note the lack of "-devel" packages.
RUN dnf install -y \
    qt5-qtbase \
    qt5-qtwayland \
    cryptopp && dnf clean all

# Set the Wayland environment variable so Qt works
ENV QT_QPA_PLATFORM=wayland

WORKDIR /app

# The magic step: Copy ONLY the compiled executables from the "builder" stage
COPY --from=builder /app/build/lamport-auth-gui .
COPY --from=builder /app/build/lamport-server-console .
COPY --from=builder /app/build/lamport-client-console .

# Also copy the config file
COPY config.json .

# Set the default command
CMD ["./lamport-auth-gui"]

