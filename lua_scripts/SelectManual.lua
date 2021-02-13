-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

Title       = "Manual"
Description = "Select manually"

function Select(Files,Times,Size)
  -- Files : table of filenames.
  -- Times : corresponding table of modification times. 
  -- Size  : filesize.
  local Selected = {}
  for i=1,#Files do
    Selected[i] = false
  end
  return Selected
end

-------------------------------------------------------------------------------
