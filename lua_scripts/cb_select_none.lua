-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "Deselect all"
description = "Deselect all"

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times. 
  -- ctimes : corresponding table of creation times. 
  -- inodes : corresponding table of (fake) inodes. 
  -- size  : filesize.
  local selected = {}
  for i=1, #files do
    selected[i] = false
  end
  return selected
end

-------------------------------------------------------------------------------
