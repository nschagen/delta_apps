# delta_apps

Contains apps that can be fuzzed and then analyzed using my delta framework.

A special version of [VUzzer](https://github.com/vusec/vuzzer) is used to accomplish this. It requires each app to have a session_setup.py in order 
to deliver a fuzzed payload. Each app has a discriminator.py script which generates a discriminator, which serves as an example for what the fuzzer tries to find.

This repository needs to be cloned inside the 'apps' directory of the llvm-apps repo to work properly.
