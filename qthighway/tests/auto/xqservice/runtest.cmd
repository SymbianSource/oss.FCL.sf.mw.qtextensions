@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
@rem All rights reserved.
@rem
@rem This program is free software: you can redistribute it and/or modify
@rem it under the terms of the GNU Lesser General Public License as published by
@rem the Free Software Foundation, version 2.1 of the License.
@rem 
@rem This program is distributed in the hope that it will be useful,
@rem but WITHOUT ANY WARRANTY; without even the implied warranty of
@rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
@rem GNU Lesser General Public License for more details.
@rem
@rem You should have received a copy of the GNU Lesser General Public License
@rem along with this program.  If not, 
@rem see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
@rem
@rem Description:
@rem

@echo on

call del %1.log

:FINAL
call \epoc32\release\winscw\udeb\%1.exe -o c:\%1.log
call copy \epoc32\winscw\c\%1.log .
call type %1.log