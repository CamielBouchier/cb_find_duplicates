-------------------------------------------------------------------------------
--
-- $BeginLicense$
-- $EndLicense$
--
-------------------------------------------------------------------------------

Title       = "Contains 'Build'"
Description = "Select those containing 'Buid' in path"

function Select(Files,Times,Size)
  -- Files : table of filenames.
  -- Times : corresponding table of modification times. 
  -- Size  : filesize.
  local Selected = {}
  for i=1,#Files do
    Selected[i] = (nil ~= string.find(Files[i],"Build"))
  end
  AllSelected = true
  for i=1,#Files do
    if not Selected[i] then
      AllSelected = false
      break
    end
  end
  if AllSelected then
    Selected[1] = false
  end
  return Selected
end

-------------------------------------------------------------------------------
