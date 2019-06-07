# Naming semantics for assets in the artwork folder
- Each asset file is postfixed with a number. The number denotes the n'th iteration of postprocessing
- The file with the postfix 0 is the original file without any postprocessing applied
- If applicable, the license file for a given file has the same name and is prefixed with 'LICENSE_'

## Example
- We download an image 'Hero.bmp', together with a license file MyLicense.txt
- We copy the image to Artwork/Textures/Hero0.bmp and the license file to Artwork/Textures/LICENSE_Hero.png
- We apply some cutting, recoloring and format change to the hero, which gives us Hero1.png
- The final result Hero1.png is copied to bin/Assets/Textures/Hero.png 