Implementation Design
=====================

## Goals
Make sure that all variables are naturally aligned with the architecture.

Use lock free algorithms wherever possible.

Use atomic operations wherever possible

Align LogIndez and make it CAS

Do and acquire, or release, when setting the ready flag. By setting a half memory variable barrier.

Look into opportunities for static assertions

