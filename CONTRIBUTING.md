# Contributions to olcPGEX_AnimatedSprite

Contributions are more than welcome, and can be in the form of raising issues and feature requests or creating pull requests to add/improve features or fix issues.

## Raising issues or feature requests

Please use the provided templates to ensure that sufficient information is supplied for us to help you. The more relevant information you can provide, the easier it will be to identify the problem and quickly apply for a fix for it.

## Contributing code changes

Code changes should be submitted as merge requests from a fork of the repo.

Code changes are ideally prioritised by existing issues and feature requests. If submitting a code change that doesn't relate to an existing issue or feature request, please create one for the pull request.

Pull request descriptions or commits should properly reference the associated issue, using keywords described in the GitHub [documentation](https://help.github.com/en/github/managing-your-work-on-github/closing-issues-using-keywords).

### Code style

The following code style guidelines should be adhered to:

- **Casing:**
    - Classes and methods should use `PascalCase`
    - Variables should use `camelCase`
    - Variable names used in both the OLC Pixel Game Engine and the AnimatedSprite extension (and for the same purpose) should match, including the use of Hungarian Notation. For example, `fElapsedTime`

- **Braces:**
    - Opening braces for methods should be placed on a new line
    - Opening braces for control structures should be placed on the same line
    - Closing braces should always be on a new line
    - Example:
    ```cpp
    int foo()
    {
        if (foo) {
            //
        } else if (bar) {
            //
        } else {
            //
        }
    }
    ```
