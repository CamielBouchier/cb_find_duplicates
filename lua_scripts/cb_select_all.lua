-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

title       = "All minus first"
description = "Select all minus first"

function select(files, mtimes, ctimes, inodes, size)
  -- files : table of filenames.
  -- mtimes : corresponding table of modification times. 
  -- ctimes : corresponding table of creation times. 
  -- inodes : corresponding table of (fake) inodes. 
  -- size  : filesize.
  local selected = {}
  for i=2, #files do
    selected[i] = true
  end
  return selected
end

-------------------------------------------------------------------------------
