# NTCIPReqView
This repository provides the source files for [ReqView] (https://www.reqview.com) used to assist in generating NTCIP standards and ISO 26048-1, especially the traceability tables.

The projects.json file and documents directory provide the raw ReqView data files. The custom_templates directory contains templates used to export the contents of the project into various formats (e.g., PRL, RTM, etc.). 

The custom_scripts directory contains a program that was used to strip the text fields and other information from the more complete version of the project files. At one point, the project contained the text for each entry; however, as the final product was maintained in a separate file (e.g., Word document), the text became out-of-sync and was therefore deleted from these files.
