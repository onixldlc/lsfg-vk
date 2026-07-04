FROM debian:bookworm

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        git curl ca-certificates \
        llvm clang clang-tools clang-tidy \
        cmake ninja-build pkg-config \
        libvulkan-dev \
        mesa-common-dev \
        qt6-base-dev qt6-base-dev-tools \
        qt6-tools-dev qt6-tools-dev-tools \
        qt6-declarative-dev qt6-declarative-dev-tools \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . /src

RUN cmake -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr/local \
        -DCMAKE_CXX_COMPILER=clang++ \
        -DLSFGVK_BUILD_UI=On \
        -DLSFGVK_INSTALL_XDG_FILES=On \
    && cmake --build build \
    && cmake --install build

ENV QT_QPA_PLATFORM=offscreen

CMD ["/usr/local/bin/lsfg-vk-cli", "--help"]
