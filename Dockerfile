# Dockerfile for building and running the project main & tests.

FROM gcc:latest

RUN apt-get update && apt-get install -y cmake

COPY . /usr/src/proj_ex2

WORKDIR /usr/src/proj_ex2/src/build

RUN cmake .. && make

WORKDIR /usr/src/proj_ex2/src/exe

CMD ["/usr/bin/bash"]
# Run interactive shell for testing inside docker container.

# Command to create a docker image (run in proj_ex2 directory):
# 'docker build -t proj_ex2 .'

# Command to run:
# 'docker run -it -p 8000:9000 --name my_container proj_ex2'

# Command to launch program:
# './ids_tests'
# './command_tests'
# './main'

# Command to launch to open another bash inside the container:
# 'docker exec -it my_container bash'

# Command to exit:
# 'exit'

# Command to launch Docker Container again:
# 'docker start -ai my_container'