### What is in this folder?

- `Doxyfile`: is the doxygen configuration file that is used to generate the `ofSite` compatible documentation.  This configuration integrates a custom doxygen style by incorporating the `header.html`, `footer.html` and `doxygen.css` files also included in this directory. 
- `Doxyfile_local`: is the doxygen configuration file that is used to generate documentation without the custom style.  Currently the `header.html` and `footer.html` files include template information that cannot be rendered without the `ofSite` framework in place.  Developers who wish to test their documentation syntax, are encouraged to use this configuration file to generate documentation on their local machine to check for errors.

For more information about our doxygen style and debugging workflow, please see the wiki page:

<https://github.com/openframeworks/openFrameworks/wiki/openFrameworks-doxygen-documentation-style-guidelines>


