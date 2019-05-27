# SecureMem
Provides means of getting memory-locked pages and securely zeroing this memory for quickly-used sensitive data.

While this will take care of locking the data in memory and wiping when freed, additional 
precautions must be taken. The core size should be set to 0, so if a coredump occurs, memory won't 
be sent to disk. In addition, a signal handler should be created to catch all signals so this 
memory can be securely freed on a signal. Note also that this will not protect the memory from 
being transferred to disk upon hibernation in the case of a laptop.
