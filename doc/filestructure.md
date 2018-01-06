# File stucture
* _(int32 + string)_ magic code
* _(int32 + string)_ version
* _(int32)_ process index  
* _(int32)_ process array length
    * **process:**
    * _(int32)_ process id
    * _(int32 + wstring)_ process name
    * _(int32 + wstring)_ process path
* _(int32)_ process title index
* _(int32)_ process title array length
    * **process:**
    * _(int32)_ process id
    * _(int32)_ title array length
        * **window title:**
        * _(int32)_ title id
        * _(int32 + wstring)_ name of process title
* _(int32)_ day array length
    * **day:**
    * _(int32)_ number of entries
        * **entry:**
        * _(int32)_ field (same as last, extension of last (no new start time), etc)
        * _(QTime)_ entry start time
        * _(QTime)_ entry end time
        * _(int32)_ processes array length
            * _(int32)_ window id
