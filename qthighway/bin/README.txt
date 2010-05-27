XQ Service registration

For original SERVICE_INFO's opaque_data format (Deprecated starting from wk20)
- Copy service.prf file in the folder \epoc32\tools\qt\mkspecs\features\symbian.
The setserviceprf.cmd does this for you.

For new SERVICE_INFO's opaque_data format
- Copy service2.prf file to \epoc32\tools\qt\mkspecs\features\symbian\service.prf
The setserviceprf2.cmd does this for you.

Note !  The QtHighway MCL wk20 supports both the formats.