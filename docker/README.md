The following commands should provide a general idea how to build and upload
a base image used in CI

    git clone https://github.com/star-bnl/star-sw.git
    docker buildx build -t ghcr.io/star-bnl/star-sw-base-root5 --push \
      --cache-from=type=registry,ref=ghcr.io/star-bnl/star-sw-base-root5 \
      --cache-to=type=inline,mode=max \
      -f star-sw/docker/Dockerfile.root5 star-sw/
